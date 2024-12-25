#include	"config.h"
#include	"USART1.h"
#include  "GPIO.h"
#include  "timer.h"
#include	"delay.h"
#include  "Exti.h"
#include  "ADC.h"
#include  "compare.h"
#include	"PCA.h"
/*************	功能说明	**************



************************************/


/*************	本地常量声明	**************/


/*************	本地变量声明	**************/
// 当前的值
volatile u8 pwmPercent = 0;
// 运行速度（ 高电平大小 ）
volatile u16 pwmSpeed = 0;
// 速度周期 （ 总电平周期 ）
volatile u16 pwmCycle = 0;
// 相位状态
volatile char current_step = 1;
// 是否正常运行状态
volatile char isRun = 0;
// 运行条件  pwmSpeed 大于 runCondition 时开始运行
volatile u16 runCondition = 10;

// 运行速度
volatile u16 runSpeed = 0;



/*************	本地函数声明	**************/
void	UART_config(void);
void EXTI_config(void);
void Timer_config(void);
void GPIO_config(void);
void ADC_config(void);
void CMP_config(void);
void PWM_config(void);

// ====
void Commutation(void);
void startMotor(void);
void stopMotor(void);
void printNumber(u16 number);
/*************  外部函数和变量声明 *****************/



/**********************************************/
void main(void)
{
	UART_config();
	GPIO_config();
	PWM_config();
	EXTI_config();
	Timer_config();
	ADC_config();
	CMP_config();
	EA = 1;
	// ==========================================
	while (1)
	{
		// 判断占空比大于5
		if(pwmPercent > runCondition ){
			if(isRun){
				// 统计转速
				PrintString1("pwm = ");
				printNumber(runSpeed);
				PrintString1("\r\n");
				runSpeed = 0;
				delay_ms(1000);
			} else {
				// 当占空比大于5 开始启动
				startMotor();
			}
		}else if(isRun){
			stopMotor();
		}
		PrintString1("pwm = ");
		printNumber(pwmPercent);
		PrintString1("\r\n");
		delay_ms(100);
	}
}

/**
* 启动电机: 加速运行
*/
void startMotor(void){
	PrintString1("start !!!! ");
	while(1){
		// 判断占空比大于5
    if(runCondition < pwmPercent && isRun == 0){
			  // 判断是否读取到过零检测
			  if(0){
					// 转到正常运行
					isRun = 1;
					break;
				}else{
					// 继续加速
					Commutation();
					PrintString1("step = ");
					printNumber(current_step);
					PrintString1("\r\n");
					delay_ms(2000);
				}
		}else{
			// 停止
			stopMotor();
			break;
		}
	}
	
	CMP_start(ENABLE);
	
}

void stopMotor(void)
{
	isRun = 0;
	pwmPercent = 0;
	CMP_start(DISABLE);
	ADC_start(DISABLE);
	// 电机PMos管关闭
	UpdatePwm(PCA0, 0);
	UpdatePwm(PCA1, 0);
	UpdatePwm(PCA2, 0);
	// 电机NMos管关闭
	P33 = 0;
	P55 = 0;
	P36 = 0;
	PrintString1("stop!!!!!\r\n");
}


// 切换到下一步换相
void Commutation(void) {
		runSpeed++;
		current_step += 1;
		if(current_step>6){current_step = 1;}
		switch (current_step) {
      case 1: 
			  // U相高，V相低
				P33 = 0;
				P55 = 1;
				P36 = 0;
				UpdatePwm(PCA0, pwmPercent);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, 0);
				//ADC输入
				ADC_select(ADC_P15);
				ADC_start(ENABLE);
				// 使能下降沿中断
				CMP_HL(DISABLE);
				break;
      case 2: 
				// U相高，W相低
				P33 = 0;
				P55 = 0;
				P36 = 1;
				UpdatePwm(PCA0, pwmPercent);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, 0);
				//ADC输入
				ADC_select(ADC_P14);
				ADC_start(ENABLE);
				// 使能上升沿中断
				CMP_HL(ENABLE);
				break;
      case 3: 
				// V相高，W相低
				P33 = 0;
				P55 = 0;
				P36 = 1;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, pwmPercent);
				UpdatePwm(PCA2, 0);
				//ADC输入
				ADC_select(ADC_P13);
				ADC_start(ENABLE);
				// 使能下降沿中断
				CMP_HL(DISABLE);
				break;
      case 4: 
				// V相高，U相低
				P33 = 1;
				P55 = 0;
				P36 = 0;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, pwmPercent);
				UpdatePwm(PCA2, 0);
				//ADC输入
				ADC_select(ADC_P15);
				ADC_start(ENABLE);
				// 使能上升沿中断
				CMP_HL(ENABLE);
				break;
      case 5: 
				// W相高，U相低
				P33 = 1;
				P55 = 0;
				P36 = 0;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, pwmPercent);
				//ADC输入
				ADC_select(ADC_P14);
				ADC_start(ENABLE);
				// 使能下降沿中断
				CMP_HL(DISABLE);
				break;
      case 6: 
				// W相高，V相低
				P33 = 0;
				P55 = 1;
				P36 = 0;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, pwmPercent);
				//ADC输入
				ADC_select(ADC_P13);
				ADC_start(ENABLE);
				// 使能上升沿中断
				CMP_HL(ENABLE);
				break;
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

/**
*   通过定时器输入捕获功能来检测 PWM 信号
*/
void External0_ISR(void) interrupt INT0_VECTOR {
	if (IT0) {
		//计算占空比
		if(pwmSpeed > 0 && pwmCycle >0 && pwmCycle >= pwmSpeed){
			pwmPercent = (pwmSpeed * 256) / pwmCycle;
		}
		pwmCycle = 0;
		pwmSpeed = 0;
		// 切换为下降沿触发
		IT0 = 0;
	} else {	
		// 切换为上升沿触发
		IT0 = 1;
	}
}


/********************* Timer0中断函数************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	pwmCycle ++;
	if(IT0 != 0){
	 pwmSpeed ++;
	}
	// 计数超时 ( 1s 内没有pwm重置 )
	if(pwmCycle >= 10000){
		PrintString1("bb");
		pwmCycle = 0;
		pwmSpeed = 0;
		stopMotor();
	}
}


/********************* CMP 中断函数************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	PrintString1("CMP!!!!!\r\n");
	//清除中断标志
	CMPCR1 &= ~CMPIF;
	PrintString1("CMP = ");
	PrintString1("\r\n");
	// Commutation();
}


/********************* 描述: ADC中断函数.************************/
void ADC_int (void) interrupt ADC_VECTOR
{
	//清除标志
	ADC_CONTR &= ~ADC_FLAG;
	//启动ADC转换
	ADC_start(ENABLE);
}


/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/


/******     串口1初始化函数  ********/
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
	delay_ms(500);
	PrintString1(" STC15 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
}



/******     pwm     ******/

void PWM_config(void){

	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_12T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P12_P11_P10_P37;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityLow;			//优先级设置	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PWM_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Value    = 0 ;			//对于PWM,高8位为PWM占空比
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PWM_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Value    = 0 ;			//对于PWM,高8位为PWM占空比
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PWM_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Value    = 0 ;			//对于PWM,高8位为PWM占空比
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PWM_Init(PCA2,&PCA_InitStructure);
	
	UpdatePwm(PCA0, 0);
	UpdatePwm(PCA1, 0);
	UpdatePwm(PCA2, 0);
	CR = 1;
}




/******     pwm     ******/
void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}




/******     IO口配置     **********/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_2;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
	
	
	// 电机PMos管关闭
	P33 = 0;
	P11 = 0;
	P55 = 0;
	
	// 电机NMos管关闭
	P10 = 0;
	P37 = 0;
	P36 = 0;
	
}


/********       定时器配置  ***************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	// ==== 0
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - 200;		//初值,
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2

}

/********       ADC  ***************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //结构定义
	ADC_InitStructure.ADC_Px        = ADC_P13 | ADC_P14 | ADC_P15;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		                //优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //初始化
	ADC_PowerControl(ENABLE);							                          //单独的ADC电源操作函数, ENABLE或DISABLE
}


/************************ 比较器配置 ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//结构定义
	CMP_InitStructure.CMP_EN = DISABLE;					//允许比较器		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = DISABLE;		//允许上升沿中断	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = DISABLE;		//允许下降沿中断	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_ADCIS;		//比较器输入正极性选择, CMP_P_P55: 选择内部P5.5做正输入, CMP_P_ADCIS: 由ADCIS[2:0]所选择的ADC输入端做正输入.
	CMP_InitStructure.CMP_N_Select     = CMP_N_P54;		//比较器输入负极性选择, CMP_N_BGv: 选择内部BandGap电压BGv做负输入, CMP_N_P54: 选择外部P5.4做输入.
	CMP_InitStructure.CMP_OutptP12_En  = DISABLE;		//允许比较结果输出到P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//比较器输出取反, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;		//内部0.1uF滤波,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 60;			//比较结果变化延时周期数, 0~63
  CMP_InitStructure.CMP_Polity	   = PolityHigh;	//中断优先级,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);
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
