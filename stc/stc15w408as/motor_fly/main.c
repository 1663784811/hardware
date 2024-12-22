#include	"config.h"
#include  "GPIO.h"
#include  "timer.h"
#include	"delay.h"
#include  "Exti.h"
#include  "ADC.h"
#include  "compare.h"

/*************	����˵��	**************
˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����
ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/



/*************	���س�������	**************/


/*************	���ر�������	**************/
// �ߵ�ƽռ�ձ�
volatile u8 pwmPercent = 0;
// ��ռ�ձ�
volatile u8 pwmPercentAll = 100;
// ��ǰ��ֵ
volatile u8 nowSpeed = 0;


// ��λ״̬
volatile char current_step = 1;
// �Ƿ���������״̬
volatile char isRun = 0;

// ��������  pwmSpeed ���� runCondition ʱ��ʼ����
volatile u16 runCondition = 5;




// �����ٶȣ� �ߵ�ƽ��С ��
volatile u16 pwmSpeed = 0;
// �ٶ����� �� �ܵ�ƽ���� ��
volatile u16 pwmCycle = 0;


/*************	���غ�������	**************/
void EXTI_config(void);
void Timer_config(void);
void GPIO_config(void);
void ADC_config(void);
void CMP_config(void);
// ====
void Commutation(void);
void startMotor(void);
void stopMotor(void);
/*************  �ⲿ�����ͱ������� *****************/



/**********************************************/
void main(void)
{
	GPIO_config();
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
				
			} else {
				// ��ռ�ձȴ���5 ��ʼ����
				startMotor();
			}
		}else{
			isRun = 0;
		}
		delay_ms(1000);
	}
}

/**
* �������: ��������
*/
void startMotor(void){
	while(1){
		// �ж�ռ�ձȴ���5
    if(runCondition < pwmSpeed && isRun == 0){
			  // �ж��Ƿ��ȡ��������
			  if(0){
					// ת����������
					isRun = 1;
					
				}else{
					// ��������
					Commutation();
					delay_ms(10);
				}
		}else{
			// ֹͣ
			break;
		}
	}
}




// �л�����һ������
void Commutation(void) {
		// �رն�ʱ��2
		current_step += 1;
		if(current_step>6){current_step = 1;}
		switch (current_step) {
      case 1: 
			  // U��ߣ�V���
				P33 = 0;
				P10 = 0;
				P11 = 1;
			  P37 = 1;
			  P55 = 1;
			  P36 = 0;
				//ADC����
				ADC_select(ADC_P15);
				ADC_start();
				// ʹ���½����ж�
				CMP_RiseInterruptEn(DISABLE);
			  CMP_FallInterruptEn(ENABLE);
				break;
      case 2: 
				// U��ߣ�W���
				P33 = 0;
				P10 = 0;
				P11 = 1;
			  P37 = 0;
			  P55 = 1;
			  P36 = 1;
				//ADC����
				ADC_select(ADC_P14);
				ADC_start();
				// ʹ���������ж�
				CMP_RiseInterruptEn(ENABLE);
			  CMP_FallInterruptEn(DISABLE);
				break;
      case 3: 
				// V��ߣ�W���
				P33 = 1;
				P10 = 0;
				P11 = 0;
			  P37 = 0;
			  P55 = 1;
			  P36 = 1;
				//ADC����
				ADC_select(ADC_P13);
				ADC_start();
				// ʹ���½����ж�
				CMP_RiseInterruptEn(DISABLE);
			  CMP_FallInterruptEn(ENABLE);
				break;
      case 4: 
				// V��ߣ�U���
				P33 = 0;
				P10 = 1;
				P11 = 0;
			  P37 = 0;
			  P55 = 1;
			  P36 = 1;
				//ADC����
				ADC_select(ADC_P15);
				ADC_start();
				// ʹ���������ж�
				CMP_RiseInterruptEn(ENABLE);
			  CMP_FallInterruptEn(DISABLE);
				break;
      case 5: 
				// W��ߣ�U���
				P33 = 0;
				P10 = 1;
				P11 = 1;
			  P37 = 1;
			  P55 = 0;
			  P36 = 0;
				//ADC����
				ADC_select(ADC_P14);
				ADC_start();
				// ʹ���½����ж�
				CMP_RiseInterruptEn(DISABLE);
			  CMP_FallInterruptEn(ENABLE);
				break;
      case 6: 
				// W��ߣ�V���
				P33 = 1;
				P10 = 1;
				P11 = 0;
			  P37 = 1;
			  P55 = 0;
			  P36 = 0;
				//ADC����
				ADC_select(ADC_P13);
				ADC_start();
				// ʹ���������ж�
				CMP_RiseInterruptEn(ENABLE);
			  CMP_FallInterruptEn(DISABLE);
				break;
    }
}


void stopMotor(void)
{
	isRun = 0;
	pwmPercent = 0;
	// ���PMos�ܹر�
	P33 = 1;
	P11 = 1;
	P55 = 1;
	// ���NMos�ܹر�
	P10 = 1;
	P37 = 0;
	P36 = 0;
	
	P12 = 0;
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
			 pwmPercent = (pwmSpeed * 100) / pwmCycle;
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
	if(IT0 == 0){
	 pwmSpeed ++;
	}
	// ������ʱ ( 1s ��û��pwm���� )
	if(pwmCycle >= 2000){
	  pwmCycle = 0;
		pwmSpeed = 0;
	  stopMotor();
	}
}

/********************* Timer1�жϺ���  ********************/
void timer2_int (void) interrupt TIMER2_VECTOR
{
	nowSpeed++;
	if( nowSpeed <= pwmPercent){
		if(current_step == 1 || current_step == 2){
			P33 = 0;
		}else if(current_step == 3 || current_step == 4){
			P11 = 0;
		}else if(current_step == 5 || current_step == 6){
			P55 = 0;
		}
	}else{
		if(current_step == 1 || current_step == 2){
			P33 = 1;
		}else if(current_step == 3 || current_step == 4){
			P11 = 1;
		}else if(current_step == 5 || current_step == 6){
			P55 = 1;
		}
	}
	P12 = ~P12;
	// ����
	if(nowSpeed >= pwmPercentAll){
		nowSpeed = 0;
	}
}



/********************* CMP �жϺ���************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	//����жϱ�־
	CMPCR1 &= ~CMPIF;
	// Commutation();
}


/********************* ����: ADC�жϺ���.************************/
void ADC_int (void) interrupt ADC_VECTOR
{
	//�����־
	ADC_CONTR &= ~ADC_FLAG;
	//����ADCת��
	ADC_start();
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




/******     IO������     **********/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//��ʼ��
	
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
	
	
	// ���PMos�ܹر�
	P33 = 1;
	P11 = 1;
	P55 = 1;
	
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
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - 1000;		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2

	// === 2
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL-80;		//��ֵ,
	TIM_InitStructure.TIM_Run       = DISABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//��ʼ��Timer1	  Timer0,Timer1,Timer2
}

/********       ADC  ***************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P13 | ADC_P14 | ADC_P15;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		                //���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //��ʼ��
	ADC_PowerControl(ENABLE);							                          //������ADC��Դ��������, ENABLE��DISABLE
}


/************************ �Ƚ������� ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//�ṹ����
	CMP_InitStructure.CMP_EN = ENABLE;					//����Ƚ���		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = DISABLE;		//�����������ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = DISABLE;		//�����½����ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_ADCIS;		//�Ƚ�������������ѡ��, CMP_P_P55: ѡ���ڲ�P5.5��������, CMP_P_ADCIS: ��ADCIS[2:0]��ѡ���ADC�������������.
	CMP_InitStructure.CMP_N_Select     = CMP_N_P54;		//�Ƚ������븺����ѡ��, CMP_N_BGv: ѡ���ڲ�BandGap��ѹBGv��������, CMP_N_P54: ѡ���ⲿP5.4������.
	CMP_InitStructure.CMP_OutptP12_En  = DISABLE;		//����ȽϽ�������P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//�Ƚ������ȡ��, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = DISABLE;		//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 0;			//�ȽϽ���仯��ʱ������, 0~63
  CMP_InitStructure.CMP_Polity	   = PolityLow;	//�ж����ȼ�,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);
}
