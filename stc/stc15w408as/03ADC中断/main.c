#include	"config.h"
#include	"delay.h"
#include	"USART1.h"
#include  "ADC.h"
/*************	����˵��	**************

��������ʾ��·ADC��ѯ������ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��
�û������޸�Ϊ1~8·��ADCת����

******************************************/

#define ADC_POWER 0x80 // ADC��Դ����λ


#define ADC_SPEEDLL 0x00 // 540��ʱ��


#define ADC_SPEEDL 0x20  // 360��ʱ��
#define ADC_SPEEDH 0x40  // 180��ʱ��
#define ADC_SPEEDHH 0x60 // 90��ʱ��
#define VCC 5000         // set ADC_REF_VCC

typedef unsigned char BYTE;
typedef unsigned int WORD;

BYTE ch = 1;   // ADCͨ����1����LM35�������ź��������
BYTE flag = 1; // ����ת����ɱ�־λ
BYTE H8bit = 0, L2bit = 0;
WORD AD10bit = 0, temp = 0, dat = 0, dat1 = 0;
unsigned long result = 0;



u16 test = 0;



/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/
void UART_config(void);
void	ADC_config(void);


void printNumber(u16 number);
/*************  �ⲿ�����ͱ������� *****************/

void InitADC(void);


/**********************************************/
void main(void)
{
	EA = 1;
	UART_config();
	ADC_config();
	while (1)
	{
		//����ADCת��
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


/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/
/************************************     �ж�     ***************************************/



//========================================================================
// ����: void ADC_int(void) interrupt ADC_VECTOR
// ����: ADC�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2012-10-22
//========================================================================
void ADC_int (void) interrupt ADC_VECTOR
{
	u16	adc;
	//���ADC�жϱ�־
	ADC_CONTR &= ~ADC_FLAG;
	if(PCON2 &  (1<<5)){
		//10λAD����ĸ�2λ��ADC_RES�ĵ�2λ����8λ��ADC_RESL��
		adc = (u16)(ADC_RES & 3);
		adc = (adc << 8) | ADC_RESL;
	} else {
		//10λAD����ĸ�8λ��ADC_RES����2λ��ADC_RESL�ĵ�2λ��
		adc = (u16)ADC_RES;
		adc = (adc << 2) | (ADC_RESL & 3);
	}
	PrintString1("ADC= ");
	printNumber(adc);
	PrintString1("\r\n");
	flag = 1;
}









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
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H2L8;		              //ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		                //���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //�ж�����		ENABLE,DISABLE
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
