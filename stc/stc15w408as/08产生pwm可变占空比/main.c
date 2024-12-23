#include	"config.h"
#include  "Exti.h"
#include	"USART1.h"
#include	"delay.h"
#include  "timer.h"
#include  "GPIO.h"
/*************	����˵��	**************


����50Hz �ķ��� ����� P11
ռ�ձ�Ϊ��50��
���㹫ʽ�� 

******************************************/



/*************	���س�������	**************/


/*************	���ر�������	**************/
// �ߵ�ƽռ�ձ�
u8 pwmPercent = 20;
// ��ռ�ձ�
u8 pwmPercentAll = 100;
// ��ǰ��ֵ
u8 nowSpeed = 0;

/*************	���غ�������	**************/
void	EXTI_config(void);
void	UART_config(void);
void	Timer_config(void);
void	GPIO_config(void);


void printNumber(u16 number);
/*************  �ⲿ�����ͱ������� *****************/


/**********************************************/
void main(void)
{
	int aa = 0;
	GPIO_config();
	EXTI_config();
	// UART_config();
	Timer_config();
	EA = 1;             // �����ж�
	// ==========================================
	while (1)
	{
		if(aa){
			pwmPercent ++;
			if(pwmPercent>=100){
				aa = 0;
			}
		}else{
			pwmPercent --;
			if(pwmPercent<=0){
				aa = 1;
			}
		}
		delay_ms(500);
	}
}

/**
*   ͨ����ʱ�����벶��������� PWM �ź�
*/
void External0_ISR(void) interrupt INT0_VECTOR {
    if (IT0) {
			// �л�Ϊ�½��ش���
			IT0 = 0;
    } else {	
			// �л�Ϊ�����ش���
			IT0 = 1;

    }

	
}




///********************* Timer0�жϺ���  ����pwm�� ********************/
//void timer0_int (void) interrupt TIMER0_VECTOR
//{
//		P10 = ~P10;
//}



/********************* Timer1�жϺ���  ********************/
void timer2_int (void) interrupt TIMER2_VECTOR
{
	nowSpeed++;
	if( nowSpeed <= pwmPercent){
		P11 = 1;
	}else{
		P11 = 0;
	}
	// ����
	if(nowSpeed >= pwmPercentAll){
		nowSpeed = 0;
	}
	
}






/************************************     ����1��ʼ������     ***************************************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
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



/*************************************        ��ʱ������      **************************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
//	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
//	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
//	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - 20000;		//��ֵ,  
//	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
//	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
// ���ϼ��㹫ʽ��    ʱ��Դ / 12��Ƶ / 65535 ( 16λ��� ) 
//                 24000000   ��   12   ��   ��256 �� 20000�� =  100 ÿ�����ж�/��
// ���pwm�ź�һ��Ϊ 50 HZ  ��20ms      100 �� 2 = 50Hz   ÿ20ms�ж�/��
	
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL-400;		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//��ʼ��Timer1	  Timer0,Timer1,Timer2
	
}

/************************************       IO������       *****************************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//��ʼ��
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

