#include	"config.h"
#include	"USART1.h"
#include  "GPIO.h"
#include  "timer.h"
#include	"delay.h"
#include  "Exti.h"
#include  "ADC.h"
#include  "compare.h"
#include	"PCA.h"
/*************	����˵��	**************



************************************/


/*************	���س�������	**************/


/*************	���ر�������	**************/
// ��ǰ��ֵ
volatile u8 pwmPercent = 0;
// �����ٶȣ� �ߵ�ƽ��С ��
volatile u16 pwmSpeed = 0;
// �ٶ����� �� �ܵ�ƽ���� ��
volatile u16 pwmCycle = 0;
// ��λ״̬
volatile char current_step = 1;
// �Ƿ���������״̬
volatile char isRun = 0;
// ��������  pwmSpeed ���� runCondition ʱ��ʼ����
volatile u16 runCondition = 10;

// �����ٶ�
volatile u16 runSpeed = 0;



/*************	���غ�������	**************/
void	UART_config(void);
void EXTI_config(void);
void Timer_config(void);
void GPIO_config(void);
void ADC_config(void);
void CMP_config(void);
void PWM_config(void);

// ====
void Commutation(void);
void startMotor(void);
void stopMotor(void);
void printNumber(u16 number);
/*************  �ⲿ�����ͱ������� *****************/



/**********************************************/
void main(void)
{
	UART_config();
	GPIO_config();
	PWM_config();
	EXTI_config();
	Timer_config();
	ADC_config();
	CMP_config();
	EA = 1;
	// ==========================================
	while (1)
	{
		// �ж�ռ�ձȴ���5
		if(pwmPercent > runCondition ){
			if(isRun){
				// ͳ��ת��
				PrintString1("pwm = ");
				printNumber(runSpeed);
				PrintString1("\r\n");
				runSpeed = 0;
				delay_ms(1000);
			} else {
				// ��ռ�ձȴ���5 ��ʼ����
				startMotor();
			}
		}else if(isRun){
			stopMotor();
		}
		PrintString1("pwm = ");
		printNumber(pwmPercent);
		PrintString1("\r\n");
		delay_ms(100);
	}
}

/**
* �������: ��������
*/
void startMotor(void){
	PrintString1("start !!!! ");
	while(1){
		// �ж�ռ�ձȴ���5
    if(runCondition < pwmPercent && isRun == 0){
			  // �ж��Ƿ��ȡ��������
			  if(0){
					// ת����������
					isRun = 1;
					break;
				}else{
					// ��������
					Commutation();
					PrintString1("step = ");
					printNumber(current_step);
					PrintString1("\r\n");
					delay_ms(2000);
				}
		}else{
			// ֹͣ
			stopMotor();
			break;
		}
	}
	
	CMP_start(ENABLE);
	
}

void stopMotor(void)
{
	isRun = 0;
	pwmPercent = 0;
	CMP_start(DISABLE);
	ADC_start(DISABLE);
	// ���PMos�ܹر�
	UpdatePwm(PCA0, 0);
	UpdatePwm(PCA1, 0);
	UpdatePwm(PCA2, 0);
	// ���NMos�ܹر�
	P33 = 0;
	P55 = 0;
	P36 = 0;
	PrintString1("stop!!!!!\r\n");
}


// �л�����һ������
void Commutation(void) {
		runSpeed++;
		current_step += 1;
		if(current_step>6){current_step = 1;}
		switch (current_step) {
      case 1: 
			  // U��ߣ�V���
				P33 = 0;
				P55 = 1;
				P36 = 0;
				UpdatePwm(PCA0, pwmPercent);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, 0);
				//ADC����
				ADC_select(ADC_P15);
				ADC_start(ENABLE);
				// ʹ���½����ж�
				CMP_HL(DISABLE);
				break;
      case 2: 
				// U��ߣ�W���
				P33 = 0;
				P55 = 0;
				P36 = 1;
				UpdatePwm(PCA0, pwmPercent);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, 0);
				//ADC����
				ADC_select(ADC_P14);
				ADC_start(ENABLE);
				// ʹ���������ж�
				CMP_HL(ENABLE);
				break;
      case 3: 
				// V��ߣ�W���
				P33 = 0;
				P55 = 0;
				P36 = 1;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, pwmPercent);
				UpdatePwm(PCA2, 0);
				//ADC����
				ADC_select(ADC_P13);
				ADC_start(ENABLE);
				// ʹ���½����ж�
				CMP_HL(DISABLE);
				break;
      case 4: 
				// V��ߣ�U���
				P33 = 1;
				P55 = 0;
				P36 = 0;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, pwmPercent);
				UpdatePwm(PCA2, 0);
				//ADC����
				ADC_select(ADC_P15);
				ADC_start(ENABLE);
				// ʹ���������ж�
				CMP_HL(ENABLE);
				break;
      case 5: 
				// W��ߣ�U���
				P33 = 1;
				P55 = 0;
				P36 = 0;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, pwmPercent);
				//ADC����
				ADC_select(ADC_P14);
				ADC_start(ENABLE);
				// ʹ���½����ж�
				CMP_HL(DISABLE);
				break;
      case 6: 
				// W��ߣ�V���
				P33 = 0;
				P55 = 1;
				P36 = 0;
				UpdatePwm(PCA0, 0);
				UpdatePwm(PCA1, 0);
				UpdatePwm(PCA2, pwmPercent);
				//ADC����
				ADC_select(ADC_P13);
				ADC_start(ENABLE);
				// ʹ���������ж�
				CMP_HL(ENABLE);
				break;
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

/**
*   ͨ����ʱ�����벶��������� PWM �ź�
*/
void External0_ISR(void) interrupt INT0_VECTOR {
	if (IT0) {
		//����ռ�ձ�
		if(pwmSpeed > 0 && pwmCycle >0 && pwmCycle >= pwmSpeed){
			pwmPercent = (pwmSpeed * 256) / pwmCycle;
		}
		pwmCycle = 0;
		pwmSpeed = 0;
		// �л�Ϊ�½��ش���
		IT0 = 0;
	} else {	
		// �л�Ϊ�����ش���
		IT0 = 1;
	}
}


/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	pwmCycle ++;
	if(IT0 != 0){
	 pwmSpeed ++;
	}
	// ������ʱ ( 1s ��û��pwm���� )
	if(pwmCycle >= 10000){
		PrintString1("bb");
		pwmCycle = 0;
		pwmSpeed = 0;
		stopMotor();
	}
}


/********************* CMP �жϺ���************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	PrintString1("CMP!!!!!\r\n");
	//����жϱ�־
	CMPCR1 &= ~CMPIF;
	PrintString1("CMP = ");
	PrintString1("\r\n");
	// Commutation();
}


/********************* ����: ADC�жϺ���.************************/
void ADC_int (void) interrupt ADC_VECTOR
{
	//�����־
	ADC_CONTR &= ~ADC_FLAG;
	//����ADCת��
	ADC_start(ENABLE);
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
	PrintString1(" STC15 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
}



/******     pwm     ******/

void PWM_config(void){

	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_12T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P12_P11_P10_P37;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityLow;			//���ȼ�����	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PWM_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Value    = 0 ;			//����PWM,��8λΪPWMռ�ձ�
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PWM_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Value    = 0 ;			//����PWM,��8λΪPWMռ�ձ�
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PWM_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Value    = 0 ;			//����PWM,��8λΪPWMռ�ձ�
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PWM_Init(PCA2,&PCA_InitStructure);
	
	UpdatePwm(PCA0, 0);
	UpdatePwm(PCA1, 0);
	UpdatePwm(PCA2, 0);
	CR = 1;
}




/******     pwm     ******/
void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}




/******     IO������     **********/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_2;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//��ʼ��
	
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
	
	
	// ���PMos�ܹر�
	P33 = 0;
	P11 = 0;
	P55 = 0;
	
	// ���NMos�ܹر�
	P10 = 0;
	P37 = 0;
	P36 = 0;
	
}


/********       ��ʱ������  ***************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	// ==== 0
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - 200;		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2

}

/********       ADC  ***************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P13 | ADC_P14 | ADC_P15;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityHigh;		                //���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //��ʼ��
	ADC_PowerControl(ENABLE);							                          //������ADC��Դ��������, ENABLE��DISABLE
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
	CMP_InitStructure.CMP_OutptP12_En  = DISABLE;		//����ȽϽ�������P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//�Ƚ������ȡ��, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;		//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 60;			//�ȽϽ���仯��ʱ������, 0~63
  CMP_InitStructure.CMP_Polity	   = PolityHigh;	//�ж����ȼ�,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);
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
