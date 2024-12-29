#include	"config.h"
#include	"compare.h"
#include	"GPIO.h"
#include  "delay.h"
#include  "ADC.h"
#include	"USART1.h"

/*************	功能说明	**************

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/
void	UART_config(void);
void	CMP_config(void);
void	ADC_config(void);

void printNumber(u16 number);

/******************** task A **************************/
void main(void)
{
	int aa = 0;
	EA = 1;
	UART_config();
  ADC_config();
	CMP_config();

	
	// -----------------------------------------
	ADC_select(ADC_CH0);
	CMP_HL(ENABLE);
	// -----------------------------------------
	ADC_start(ENABLE);
	CMP_start(ENABLE);
	// 使能下降沿中断
	while (1)
	{
		
		if(aa){
			aa = 0;
			PrintString1("ADC1\r\n");
			ADC_select(ADC_CH1);
			ADC_start(ENABLE);
			CMP_start(ENABLE);
		}else {
			aa = 1;
			PrintString1("ADC0\r\n");
			ADC_select(ADC_CH0);
			ADC_start(ENABLE);
			CMP_start(ENABLE);
		}
		delay_ms(10000);
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

//========================================================================
// 函数: void ADC_int(void) interrupt ADC_VECTOR
// 描述: ADC中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2012-10-22
//========================================================================
/********************* CMP 中断函数************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	//清除中断标志
	CMPCR1 &= ~CMPIF;
	PrintString1("CMP= \r\n");
	CMP_start(DISABLE);
}


/********************* 描述: ADC中断函数.************************/
void ADC_int (void) interrupt ADC_VECTOR
{
	u16	adc;
	//清除标志
	ADC_CONTR &= ~ADC_FLAG;
	// 读取ADC电压
	
	if(PCON2 &  (1<<5)){
		//10位AD结果的高2位放ADC_RES的低2位，低8位在ADC_RESL。
		adc = (u16)(ADC_RES & 3);
		adc = (adc << 8) | ADC_RESL;
	} else {
		//10位AD结果的高8位放ADC_RES，低2位在ADC_RESL的低2位。
		adc = (u16)ADC_RES;
		adc = (adc << 2) | (ADC_RESL & 3);
	}

	PrintString1("ADC=");
	printNumber(adc);
	PrintString1("\r\n");
	//启动ADC转换
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
	PrintString1("STC15!\r\n");	//SUART1发送一个字符串
	delay_ms(500);
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
	CMP_InitStructure.CMP_OutptP12_En  = ENABLE;		//允许比较结果输出到P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//比较器输出取反, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;		//内部0.1uF滤波,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 60;			//比较结果变化延时周期数, 0~63
  CMP_InitStructure.CMP_Polity	   = PolityLow;	//中断优先级,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);
}


/********       ADC  ***************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //结构定义
	ADC_InitStructure.ADC_Px        = ADC_P11 | ADC_P10;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		                //优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //初始化
	ADC_PowerControl(ENABLE);							                          //单独的ADC电源操作函数, ENABLE或DISABLE
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
