#include	"config.h"
#include  "GPIO.h"
#include  "timer.h"
#include	"USART1.h"
#include	"delay.h"
#include  "Exti.h"

/*************	����˵��	**************
˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����
ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/



/*************	���س�������	**************/

/*************	���ر�������	**************/
// ��λ״̬
volatile char current_step = 1;
// �Ƿ���������״̬
volatile char isRun = 0;

// ��������  pwmSpeed ���� runCondition ʱ��ʼ����
volatile u16 runCondition = 10;


// ��ǰ�ٶ�
volatile u16 nowSpeed = 0;
// ��ǰpwm �ٶ�����
//unsigned char *runPin = null;
// �����ٶȣ� �ߵ�ƽ��С ��
volatile u16 pwmSpeed = 0;
// �ٶ����� �� �ܵ�ƽ���� ��
volatile u16 pwmCycle = 65535;




/*************	���غ�������	**************/
void	EXTI_config(void);
//����1��ʼ������
void UART_config(void);
void Timer_config(void);
void GPIO_config(void);
void ZeroCrossDetected(void);
void Commutation(void);
void startMotor(void);

/*************  �ⲿ�����ͱ������� *****************/



/**********************************************/
void main(void)
{
	EXTI_config();
	GPIO_config();      // gpio ����
	Timer_config();     // ��ʱ������
	UART_config();
	EA = 1;             // �����ж�

	// ==========================================
	
	while (1)
	{
		if(isRun){
			// �ж�ռ�ձȴ���5
			
			if(0){
				// ������,ת��
				
				ZeroCrossDetected();		
			
			}else{
				isRun = 0;
			}
		} else {
			// ��ռ�ձȴ���5 ��ʼ����
			startMotor();
		}
	}
}

/**
* ����ռ�ձ�
*/
void inputPwm(void){


}

/**
* �������: ��������
*/
void startMotor(void){
	while(1){
		// �ж�ռ�ձȴ���5
    if(runCondition < pwmSpeed){
			  // �ж��Ƿ��ȡ��������
			
			
			  if(0){
					// ת����������
					isRun = 1;
					
					
				}else{
					// ��������
				  Commutation();
				  delay_ms(20);
				}
		}else{
			// ֹͣ
			break;
		}
	}
}


/**
*   ͨ����ʱ�����벶��������� PWM �ź�
*/
void External0_ISR(void) interrupt INT0_VECTOR {
//    if (measuring_high) {
//        IT0 = 0;  // �л�Ϊ�½��ش���
//    } else {
//        IT0 = 1;  // �л�Ϊ�����ش���
//    }
}




/**
* ��⵽����� ����ADC ����
* return �Ƿ���Ի��� 0 ��1
*/
void ZeroCrossDetected(void) {
    //Delay(�����ӳ�ʱ��);
    //Commutation();
	  // �ж��ĸ���λ�ǿյ�
	  if(0){
			
			
		}
		//return 0;
}


// �л�����һ������
void Commutation(void) {
    switch (current_step) {
      case 1: 
				//Drive(U, V);
				break;
      case 2: 
				//Drive(V, W); 
				break;
      case 3: 
				//Drive(W, U); 
				break;
      case 4: 
				//Drive(U, W); 
				break;
      case 5: 
				//Drive(W, V); 
				break;
      case 6: 
				//Drive(V, U); 
				break;
    }
		current_step += 1;
		if(current_step>6){
			current_step = 1;
		}
}




/********************* Timer0�жϺ���   ************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	nowSpeed++;
	if( nowSpeed < pwmSpeed ){
		// ��ƽ����
	  
	} else {
		// ��ƽ����
		
		

	}
	//== ��������
	if(nowSpeed >= pwmCycle){
		 nowSpeed  = 0;
	}
}

/********************* Timer1�жϺ���  ����pwm�� ********************/
void timer1_int (void) interrupt TIMER1_VECTOR
{
	//
	

}

/********************* Timer2�жϺ���  ************************/
void timer2_int (void) interrupt TIMER2_VECTOR
{

}



/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/************************************     ��ʼ��     ***************************************/
/******     pwm     ******/
void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}



/******     ����1��ʼ������     ******/
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

/******     IO������     **********/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//��ʼ��
	
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
}


/********       ��ʱ������  ***************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 100000UL);		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer1,&TIM_InitStructure);				//��ʼ��Timer1	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (50*12));		//��ֵ
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//��ʼ��Timer2	  Timer0,Timer1,Timer2
}





