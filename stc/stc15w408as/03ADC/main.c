#include	"config.h"
#include	"delay.h"
#include	"USART1.h"
#include  "ADC.h"
/*************	����˵��	**************

��������ʾ��·ADC��ѯ������ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��
�û������޸�Ϊ1~8·��ADCת����

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/
void UART_config(void);
void	ADC_config(void);


void printNumber(u16 number);
/*************  �ⲿ�����ͱ������� *****************/




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
			j = Get_ADC10bitResult(i);	//����0~7,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 1024 Ϊ����
			PrintString1("ADC = ");
			printNumber(j);
			PrintString1("\r\n");
		}
		delay_ms(2000);
	}
}


/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/



/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/



void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P10 | ADC_P11;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		                //���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			                  //�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //��ʼ��
	ADC_PowerControl(ENABLE);							                          //������ADC��Դ��������, ENABLE��DISABLE
}



/******     ����1��ʼ������  ********/
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
	delay_ms(500);
	PrintString1(" STC15 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
}


/**
** ��ӡ����
**/
void printNumber(u16 number) {
    char sum = 0;
		char c = 0;
    u16 temp = number;
    u16 temp2 = 0;
    // �ж��ж���λ
    while (temp != 0) {
        temp /= 10;
        sum++;
    }
    //��ӡ�����
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
