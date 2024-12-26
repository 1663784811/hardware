#include	"config.h"
#include	"delay.h"
#include	"USART1.h"
#include  "ADC.h"
/*************	功能说明	**************

本程序演示多路ADC查询采样，通过模拟串口发送给上位机，波特率9600,8,n,1。
用户可以修改为1~8路的ADC转换。

******************************************/

#define ADC_POWER 0x80 // ADC电源控制位


#define ADC_SPEEDLL 0x00 // 540个时钟


#define ADC_SPEEDL 0x20  // 360个时钟
#define ADC_SPEEDH 0x40  // 180个时钟
#define ADC_SPEEDHH 0x60 // 90个时钟
#define VCC 5000         // set ADC_REF_VCC

typedef unsigned char BYTE;
typedef unsigned int WORD;

BYTE ch = 1;   // ADC通道号1，接LM35传感器信号输出引脚
BYTE flag = 1; // 设置转换完成标志位
BYTE H8bit = 0, L2bit = 0;
WORD AD10bit = 0, temp = 0, dat = 0, dat1 = 0;
unsigned long result = 0;



u16 test = 0;



/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/
void UART_config(void);
void	ADC_config(void);


void printNumber(u16 number);
/*************  外部函数和变量声明 *****************/

void InitADC(void);


/**********************************************/
void main(void)
{
	EA = 1;
	UART_config();
	ADC_config();
	while (1)
	{
		//启动ADC转换
		if(flag){
			flag = 0;
			ADC_select(ADC_CH1);
			PrintString1("test= ");
			printNumber(ADC_CONTR);
			PrintString1("\r\n");
			delay_ms(1000);
			ADC_start(ENABLE);
		}
		delay_ms(2000);
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
void ADC_int (void) interrupt ADC_VECTOR
{
	u16	adc;
	//清除ADC中断标志
	ADC_CONTR &= ~ADC_FLAG;
	if(PCON2 &  (1<<5)){
		//10位AD结果的高2位放ADC_RES的低2位，低8位在ADC_RESL。
		adc = (u16)(ADC_RES & 3);
		adc = (adc << 8) | ADC_RESL;
	} else {
		//10位AD结果的高8位放ADC_RES，低2位在ADC_RESL的低2位。
		adc = (u16)ADC_RES;
		adc = (adc << 2) | (ADC_RESL & 3);
	}
	PrintString1("ADC= ");
	printNumber(adc);
	PrintString1("\r\n");
	flag = 1;
}









/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/
/************************************     初始化     ***************************************/


void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //结构定义
	ADC_InitStructure.ADC_Px        = ADC_P10 | ADC_P11;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H2L8;		              //ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		                //优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //初始化
	ADC_PowerControl(ENABLE);							                          //单独的ADC电源操作函数, ENABLE或DISABLE
}



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
