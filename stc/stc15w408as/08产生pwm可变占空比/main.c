#include	"config.h"
#include  "Exti.h"
#include	"USART1.h"
#include	"delay.h"
#include  "timer.h"
#include  "GPIO.h"
/*************	功能说明	**************


产生50Hz 的方波 输出到 P11
占空比为：50％
计算公式： 

******************************************/



/*************	本地常量声明	**************/


/*************	本地变量声明	**************/
// 高电平占空比
u8 pwmPercent = 20;
// 总占空比
u8 pwmPercentAll = 100;
// 当前的值
u8 nowSpeed = 0;

/*************	本地函数声明	**************/
void	EXTI_config(void);
void	UART_config(void);
void	Timer_config(void);
void	GPIO_config(void);


void printNumber(u16 number);
/*************  外部函数和变量声明 *****************/


/**********************************************/
void main(void)
{
	int aa = 0;
	GPIO_config();
	EXTI_config();
	// UART_config();
	Timer_config();
	EA = 1;             // 允许中断
	// ==========================================
	while (1)
	{
		if(aa){
			pwmPercent ++;
			if(pwmPercent>=100){
				aa = 0;
			}
		}else{
			pwmPercent --;
			if(pwmPercent<=0){
				aa = 1;
			}
		}
		delay_ms(500);
	}
}

/**
*   通过定时器输入捕获功能来检测 PWM 信号
*/
void External0_ISR(void) interrupt INT0_VECTOR {
    if (IT0) {
			// 切换为下降沿触发
			IT0 = 0;
    } else {	
			// 切换为上升沿触发
			IT0 = 1;

    }

	
}




///********************* Timer0中断函数  产生pwm波 ********************/
//void timer0_int (void) interrupt TIMER0_VECTOR
//{
//		P10 = ~P10;
//}



/********************* Timer1中断函数  ********************/
void timer2_int (void) interrupt TIMER2_VECTOR
{
	nowSpeed++;
	if( nowSpeed <= pwmPercent){
		P11 = 1;
	}else{
		P11 = 0;
	}
	// 重置
	if(nowSpeed >= pwmPercentAll){
		nowSpeed = 0;
	}
	
}






/************************************     串口1初始化函数     ***************************************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2
	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
}


/*************  外部函数和变量声明 *****************/

void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}



/*************************************        定时器配置      **************************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
//	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
//	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
//	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - 20000;		//初值,  
//	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
//	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
// 以上计算公式：    时钟源 / 12分频 / 65535 ( 16位溢出 ) 
//                 24000000   ÷   12   ÷   （256 － 20000） =  100 每秒钟中断/次
// 电机pwm信号一般为 50 HZ  即20ms      100 ÷ 2 = 50Hz   每20ms中断/次
	
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL-400;		//初值,
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//初始化Timer1	  Timer0,Timer1,Timer2
	
}

/************************************       IO口配置       *****************************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//初始化
}


/**
** 打印数字
**/
void printNumber(u16 number) {
    char sum = 0;
		char c = 0;
    u16 temp = number;
    u16 temp2 = 0;
    // 判断有多少位
    while (temp != 0) {
        temp /= 10;
        sum++;
    }
    //打印出结果
    while (sum > 0) {
        sum--;
        temp = sum;
        temp2 = 1;
        while (temp > 0) {
            temp2 = temp2 * 10;
            temp--;
        }
        c = (number / temp2) % 10 + '0';
        TX1_write2buff(c);
    }
}

