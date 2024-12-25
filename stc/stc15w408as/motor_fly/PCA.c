
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#include	"PCA.h"

bit		B_Capture0,B_Capture1,B_Capture2;
u8		PCA0_mode,PCA1_mode,PCA2_mode;
u16		CCAP0_tmp,PCA_Timer0;
u16		CCAP1_tmp,PCA_Timer1;
u16		CCAP2_tmp,PCA_Timer2;

/*************	功能说明	**************


******************************************/

const u16 biasPwm = 256;
u8 enablePwm0 = 0;
u8 enablePwm1 = 0;
u8 enablePwm2 = 0;

//========================================================================
// 函数: UpdatePwm(u8 PCA_id, u8 pwm_value)
// 描述: 更新PWM值. 
// 参数: PCA_id: PCA序号. 取值 PCA0,PCA1,PCA2,PCA_Counter
//		 pwm_value: pwm值, 这个值是输出低电平的时间.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void	UpdatePwm(u8 PCA_id, u8 pwm_value)
{
	if(PCA_id == PCA0){
		if(pwm_value == 0){
			CCAPM0 &= ~(1 << 1);
			enablePwm0 = 0;
		}else if(enablePwm0 == 0){
			CCAPM0 |= (1 << 1);
			enablePwm0 = 1;
		}
		CCAP0H = biasPwm - pwm_value;		
	} else if(PCA_id == PCA1){
		if(pwm_value == 0){
			CCAPM1 &= ~(1 << 1);
			enablePwm1 = 0;
		}else if(enablePwm1 == 0){
			CCAPM1 |= (1 << 1);
			enablePwm1 = 1;
		}
		CCAP1H = biasPwm - pwm_value;
	}else if(PCA_id == PCA2){
		if(pwm_value == 0){
			CCAPM2 &= ~(1 << 1);
			enablePwm2 = 0;
		}else if(enablePwm2 == 0){
			CCAPM2 |= (1 << 1);
			enablePwm2 = 1;
		}
		CCAP2H = biasPwm - pwm_value;
	}
}


//========================================================================
// 函数: void	PCA_Init(PCA_id, PCA_InitTypeDef *PCAx)
// 描述: PCA初始化程序.
// 参数: PCA_id: PCA序号. 取值 PCA0,PCA1,PCA2,PCA_Counter
//		 PCAx: 结构参数,请参考PCA.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void	PWM_Init(u8 PCA_id, PCA_InitTypeDef *PCAx)
{
	if(PCA_id > PCA_Counter)	return;		//id错误

	if(PCA_id == PCA_Counter)			//设置公用Counter
	{
		CR = 0;
		CH = 0;
		CL = 0;
		AUXR1 = (AUXR1 & ~(3<<4)) | PCAx->PCA_IoUse;			//切换IO口
		CMOD  = (CMOD  & ~(7<<1)) | PCAx->PCA_Clock;			//选择时钟源
		CMOD  = (CMOD  & ~1) | (PCAx->PCA_Interrupt_Mode & 1);	//ECF
		
		
		if(PCAx->PCA_Polity == PolityHigh)		PPCA = 1;	//高优先级中断
		else									PPCA = 0;	//低优先级中断
		return;
	}
	if(PCA_id == PCA0)
	{
		CCAPM0    = PCAx->PCA_Mode | PCAx->PCA_Interrupt_Mode;	//工作模式, 中断模式
		PCA_PWM0  = (PCA_PWM0 & ~(3<<6)) | PCAx->PCA_PWM_Wide;	//PWM宽度
	}
	if(PCA_id == PCA1)
	{
		CCAPM1    = PCAx->PCA_Mode | PCAx->PCA_Interrupt_Mode;
		PCA_PWM1  = (PCA_PWM1 & ~(3<<6)) | PCAx->PCA_PWM_Wide;
	}
	if(PCA_id == PCA2)
	{
		CCAPM2    = PCAx->PCA_Mode | PCAx->PCA_Interrupt_Mode;
		PCA_PWM2  = (PCA_PWM2 & ~(3<<6)) | PCAx->PCA_PWM_Wide;	
	}
}
