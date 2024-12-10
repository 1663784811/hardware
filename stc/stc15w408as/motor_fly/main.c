#include	"config.h"
#include  "GPIO.h"
#include  "timer.h"
#include	"USART1.h"
#include	"delay.h"
#include  "Exti.h"
#include  "ADC.h"
#include  "compare.h"

/*************	功能说明	**************
双串口全双工中断方式收发通讯程序。
通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/



/*************	本地常量声明	**************/

/*************	本地变量声明	**************/
// 相位状态
volatile char current_step = 1;
// 是否正常运行状态
volatile char isRun = 0;

// 运行条件  pwmSpeed 大于 runCondition 时开始运行
volatile u16 runCondition = 10;


// 当前速度
volatile u16 nowSpeed = 0;
// 当前pwm 速度引脚
//unsigned char *runPin = null;
// 运行速度（ 高电平大小 ）
volatile u16 pwmSpeed = 0;
// 速度周期 （ 总电平周期 ）
volatile u16 pwmCycle = 65535;




/*************	本地函数声明	**************/
void EXTI_config(void);
void UART_config(void);  //串口1初始化函数
void Timer_config(void);
void GPIO_config(void);
void ADC_config(void);
void CMP_config(void);
// ====
void ZeroCrossDetected(void);
void Commutation(void);
void startMotor(void);
void inputPwm(void);
/*************  外部函数和变量声明 *****************/



/**********************************************/
void main(void)
{
	GPIO_config();      // gpio 配置
	EXTI_config();      
	Timer_config();     // 定时器配置
	UART_config();
	EA = 1;             // 允许中断
	// ==========================================
	while (1)
	{
		if(isRun){
			// 判断占空比大于5
			
			if(0){
				// 过零检测,转向
				
				ZeroCrossDetected();		
			
			}else{
				isRun = 0;
			}
		} else {
			// 当占空比大于5 开始启动
			startMotor();
		}
	}
}

/**
* 计算占空比
*/
void inputPwm(void)
{
  PrintString1("inputPwm");
}

/**
* 启动电机: 加速运行
*/
void startMotor(void){
	while(1){
		// 判断占空比大于5
    if(runCondition < pwmSpeed){
			  // 判断是否读取到过零检测
			
			
			  if(0){
					// 转到正常运行
					isRun = 1;
					
					
				}else{
					// 继续加速
				  Commutation();
				  delay_ms(20);
				}
		}else{
			// 停止
			break;
		}
	}
}


/**
*   通过定时器输入捕获功能来检测 PWM 信号
*/
void External0_ISR(void) interrupt INT0_VECTOR {
//    if (measuring_high) {
//        IT0 = 0;  // 切换为下降沿触发
//    } else {
//        IT0 = 1;  // 切换为上升沿触发
//    }
}




/**
* 检测到过零点 采用ADC 方法
* return 是否可以换相 0 ，1
*/
void ZeroCrossDetected(void) {
    //Delay(换相延迟时间);
    //Commutation();
	  // 判断哪个相位是空的
	  if(0){
		}
		//return 0;
}


// 切换到下一步换相
void Commutation(void) {
    switch (current_step) {
      case 1: 
				//Drive(U, V);
				break;
      case 2: 
				//Drive(V, W); 
				break;
      case 3: 
				//Drive(W, U); 
				break;
      case 4: 
				//Drive(U, W); 
				break;
      case 5: 
				//Drive(W, V); 
				break;
      case 6: 
				//Drive(V, U); 
				break;
    }
		current_step += 1;
		if(current_step>6){
			current_step = 1;
		}
}



/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/************************************     中断     ***************************************/
/********************* Timer0中断函数  产生pwm波 ************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	nowSpeed++;
	if( nowSpeed < pwmSpeed ){
		// 电平拉高
	} else {
		// 电平拉低
	}
	//== 重置周期
	if(nowSpeed >= pwmCycle){
		 nowSpeed  = 0;
	}
}

/********************* Timer1中断函数  ********************/
void timer1_int (void) interrupt TIMER1_VECTOR
{

}



/********************* CMP 中断函数************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	//清除中断标志
	CMPCR1 &= ~CMPIF;
	Commutation();
}


/********************* 描述: ADC中断函数.************************/
void ADC_int (void) interrupt ADC_VECTOR
{
	//清除标志
	ADC_CONTR &= ~ADC_FLAG;
	adcRequest(ADC_P14);
}


/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/******     pwm     ******/
void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}



/******     串口1初始化函数     ******/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
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

/******     IO口配置     **********/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
}


/********       定时器配置  ***************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 100000UL);		//初值,
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);		//初值,
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer1,&TIM_InitStructure);				//初始化Timer1	  Timer0,Timer1,Timer2

//	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE. (注意: Timer2固定为16位自动重装, 中断固定为低优先级)
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (50*12));		//初值
//	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
//	Timer_Inilize(Timer2,&TIM_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2
}

/********       ADC  ***************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //结构定义
	ADC_InitStructure.ADC_Px        = ADC_P14;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		                //优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //初始化
	ADC_PowerControl(ENABLE);							                          //单独的ADC电源操作函数, ENABLE或DISABLE
}


/************************ 比较器配置 ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//结构定义
	CMP_InitStructure.CMP_EN = ENABLE;					//允许比较器		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = ENABLE;		//允许上升沿中断	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = ENABLE;		//允许下降沿中断	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_ADCIS;		//比较器输入正极性选择, CMP_P_P55: 选择内部P5.5做正输入, CMP_P_ADCIS: 由ADCIS[2:0]所选择的ADC输入端做正输入.
	CMP_InitStructure.CMP_N_Select     = CMP_N_P54;		//比较器输入负极性选择, CMP_N_BGv: 选择内部BandGap电压BGv做负输入, CMP_N_P54: 选择外部P5.4做输入.
	CMP_InitStructure.CMP_OutptP12_En  = ENABLE;		//允许比较结果输出到P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = ENABLE;		//比较器输出取反, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = DISABLE;		//内部0.1uF滤波,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 0;			//比较结果变化延时周期数, 0~63
  CMP_InitStructure.CMP_Polity	   = PolityLow;	//中断优先级,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2
}

