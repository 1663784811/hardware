#include	"config.h"
#include  "Exti.h"
#include	"USART1.h"
#include	"delay.h"


/*************	����˵��	**************


******************************************/



/*************	���س�������	**************/


/*************	���ر�������	**************/



/*************	���غ�������	**************/
void	EXTI_config(void);
//����1��ʼ������
void	UART_config(void);


/*************  �ⲿ�����ͱ������� *****************/


/**********************************************/
void main(void)
{
	EXTI_config();
	UART_config();
	EA = 1;             // �����ж�
	PrintString1("init success");
	// ==========================================
	
	while (1)
	{
		delay_ms(1000);
		PrintString1("test");
		
	}
}

/**
*   ͨ����ʱ�����벶��������� PWM �ź�
*/
void External0_ISR(void) interrupt 0 {
		PrintString1("===");	
    if (IT0) {
			PrintString1("up");
			// �л�Ϊ�½��ش���
			IT0 = 0;
    } else {
			PrintString1("down");			
			// �л�Ϊ�����ش���
			IT0 = 1;
    }
}







/************************************     ����1��ʼ������     ***************************************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2
	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
}


/*************  �ⲿ�����ͱ������� *****************/

void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}







