#include	"config.h"
#include	"compare.h"
#include	"GPIO.h"
#include  "delay.h"
#include  "ADC.h"
#include	"USART1.h"

/*************	����˵��	**************

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/
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
	// ʹ���½����ж�
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
/********************* CMP �жϺ���************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	//����жϱ�־
	CMPCR1 &= ~CMPIF;
	PrintString1("CMP= \r\n");
	CMP_start(DISABLE);
}


/********************* ����: ADC�жϺ���.************************/
void ADC_int (void) interrupt ADC_VECTOR
{
	u16	adc;
	//�����־
	ADC_CONTR &= ~ADC_FLAG;
	// ��ȡADC��ѹ
	
	if(PCON2 &  (1<<5)){
		//10λAD����ĸ�2λ��ADC_RES�ĵ�2λ����8λ��ADC_RESL��
		adc = (u16)(ADC_RES & 3);
		adc = (adc << 8) | ADC_RESL;
	} else {
		//10λAD����ĸ�8λ��ADC_RES����2λ��ADC_RESL�ĵ�2λ��
		adc = (u16)ADC_RES;
		adc = (adc << 2) | (ADC_RESL & 3);
	}

	PrintString1("ADC=");
	printNumber(adc);
	PrintString1("\r\n");
	//����ADCת��
}




/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/



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
	PrintString1("STC15!\r\n");	//SUART1����һ���ַ���
	delay_ms(500);
}


/************************ �Ƚ������� ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//�ṹ����
	CMP_InitStructure.CMP_EN = DISABLE;					//����Ƚ���		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = DISABLE;		//�����������ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = DISABLE;		//�����½����ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_ADCIS;		//�Ƚ�������������ѡ��, CMP_P_P55: ѡ���ڲ�P5.5��������, CMP_P_ADCIS: ��ADCIS[2:0]��ѡ���ADC�������������.
	CMP_InitStructure.CMP_N_Select     = CMP_N_P54;		//�Ƚ������븺����ѡ��, CMP_N_BGv: ѡ���ڲ�BandGap��ѹBGv��������, CMP_N_P54: ѡ���ⲿP5.4������.
	CMP_InitStructure.CMP_OutptP12_En  = ENABLE;		//����ȽϽ�������P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//�Ƚ������ȡ��, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;		//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 60;			//�ȽϽ���仯��ʱ������, 0~63
  CMP_InitStructure.CMP_Polity	   = PolityLow;	//�ж����ȼ�,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);
}


/********       ADC  ***************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P11 | ADC_P10;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		                //���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //��ʼ��
	ADC_PowerControl(ENABLE);							                          //������ADC��Դ��������, ENABLE��DISABLE
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
