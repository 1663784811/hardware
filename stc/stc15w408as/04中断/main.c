#include	"config.h"
#include	"USART1.h"
#include	"delay.h"


/*************	功能说明	**************


******************************************/



/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/
//串口1初始化函数
void	UART_config(void);


/*************  外部函数和变量声明 *****************/


/**********************************************/
void main(void)
{
	UART_config();
	EA = 1;             // 允许中断
	PrintString1("init success");
	// ==========================================
	
	while (1)
	{
		delay_ms(1000);
		PrintString1("test");
		
	}
}

/**
*   通过定时器输入捕获功能来检测 PWM 信号
*/
void External0_ISR(void) interrupt 0 {
    if (0) {
        //high_time = (TH0 << 8) | TL0;  // 读取高电平时间
        TH0 = 0x00;
        TL0 = 0x00;
        IT0 = 0;  // 切换为下降沿触发
    } else {
        //period_time = (TH0 << 8) | TL0;  // 读取周期时间
        TH0 = 0x00;
        TL0 = 0x00;
        IT0 = 1;  // 切换为上升沿触发
    }
}







/************************************     串口1初始化函数     ***************************************/
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





