#include	"config.h"
#include  "Exti.h"
#include	"USART1.h"
#include	"delay.h"
#include  "timer.h"
#include  "GPIO.h"
/*************	����˵��	**************


******************************************/



/*************	���س�������	**************/


/*************	���ر�������	**************/


// 
volatile double pwmPercent = 0;
// �����ٶȣ� �ߵ�ƽ��С ��
volatile u16 pwmSpeed = 0;
// �ٶ����� �� �ܵ�ƽ���� ��
volatile u16 pwmCycle = 65535;

/*************	���غ�������	**************/
void	EXTI_config(void);
//����1��ʼ������
void	UART_config(void);
// 
void	Timer_config(void);

/*************  �ⲿ�����ͱ������� *****************/


/**********************************************/
void main(void)
{
	u8 aaa = 0;
	EXTI_config();
	UART_config();
	Timer_config();
	EA = 1;             // �����ж�
	PrintString1("init success");
	// ==========================================
	
	while (1)
	{
		delay_ms(1000);
		TX1_write2buff( ((int)(pwmPercent * 10))%100/10 + '0');
		TX1_write2buff( ((int)( pwmPercent * 100 )) % 10 + '0');
		PrintString1("\r\n");
	}
}

/**
*   ͨ����ʱ�����벶��������� PWM �ź�
*/
void External0_ISR(void) interrupt INT0_VECTOR {
    if (IT0) {
			//����ռ�ձ�
			if(pwmSpeed > 0 && pwmCycle >0 && pwmCycle >= pwmSpeed ){
			   pwmPercent = pwmSpeed / pwmCycle;
			}
			//
			pwmCycle = 0;
			pwmSpeed = 0;
			// �л�Ϊ�½��ش���
			IT0 = 0;
    } else {	
			// �л�Ϊ�����ش���
			IT0 = 1;
    }
}




/********************* Timer0�жϺ���  ����pwm�� ********************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	pwmCycle ++;
	if(IT0 == 0){
	 pwmSpeed ++;
	}
	
	// ������ʱ
	if(pwmCycle >= 1600){
	  pwmCycle = 0;
		pwmSpeed = 0;
		if(IT0 == 0){
		   pwmPercent = 1;
		}else {
		   pwmPercent = 0;
		}
	}
	
}


/********************* Timer1�жϺ���   ********************/
void timer1_int (void) interrupt TIMER1_VECTOR
{
  P10 = ~P10;
	delay_ms(1000);
	
	
	
	
	
	
	
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



/*************************************        ��ʱ������      **************************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_8BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 0;		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
  
	
	
	
	
	
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;
	TIM_InitStructure.TIM_Polity    = PolityLow;
	TIM_InitStructure.TIM_Interrupt = ENABLE;
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;
	TIM_InitStructure.TIM_ClkOut    = ENABLE;
	TIM_InitStructure.TIM_Value     = 1;
	TIM_InitStructure.TIM_Run       = ENABLE;
	Timer_Inilize(Timer1,&TIM_InitStructure);

}

/************************************       IO������       *****************************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//��ʼ��
	
}

