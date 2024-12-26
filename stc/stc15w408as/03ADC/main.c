#include	"config.h"
#include	"delay.h"
#include	"USART1.h"
#include  "ADC.h"
/*************	功能说明	**************

本程序演示多路ADC查询采样，通过模拟串口发送给上位机，波特率9600,8,n,1。
用户可以修改为1~8路的ADC转换。

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/
void UART_config(void);
void	ADC_config(void);


void printNumber(u16 number);
/*************  外部函数和变量声明 *****************/




/**********************************************/
void main(void)
{
	u8	i;
	u16 j;

	UART_config();
	ADC_config();
	EA = 1;
	while (1)
	{

		for(i=0; i<2; i++)
		{
			delay_ms(250);
			j = Get_ADC10bitResult(i);	//参数0~7,查询方式做一次ADC, 返回值就是结果, == 1024 为错误
			PrintString1("ADC = ");
			printNumber(j);
			PrintString1("\r\n");
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
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		                //优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			                  //中断允许		ENABLE,DISABLE
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
