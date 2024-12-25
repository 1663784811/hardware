#include "compare.h"


//========================================================================
// 函数:void	CMP_Inilize(CMP_InitDefine *CMPx)
// 描述: 定时器初始化程序.
// 参数: CMPx: 结构参数,请参考compare.h里的定义.
// 返回: 成功返回0, 空操作返回1,错误返回2.
// 版本: V1.0, 2012-10-22
//========================================================================
void	CMP_Inilize(CMP_InitDefine *CMPx)
{
	CMPCR1 = 0;
	CMPCR2 = CMPx->CMP_OutDelayDuty & 0x3f;						//比较结果变化延时周期数, 0~63
	if(CMPx->CMP_EN == ENABLE)				CMPCR1 |= CMPEN;	//允许比较器		ENABLE,DISABLE
	if(CMPx->CMP_RiseInterruptEn == ENABLE)	CMPCR1 |= PIE;		//允许上升沿中断	ENABLE,DISABLE
	if(CMPx->CMP_FallInterruptEn == ENABLE)	CMPCR1 |= NIE;		//允许下降沿中断	ENABLE,DISABLE
	if(CMPx->CMP_P_Select  == CMP_P_ADCIS)	CMPCR1 |= PIS;		//比较器输入正极性选择, CMP_P_P55: 选择内部P5.5做正输入, CMP_P_ADCIS: 由ADCIS[2:0]所选择的ADC输入端做正输入.
	if(CMPx->CMP_N_Select  == CMP_N_P54)	CMPCR1 |= NIS;		//比较器输入负极性选择, CMP_N_BGv: 选择内部BandGap电压BGv做负输入, CMP_N_P54: 选择外部P5.4做输入.
	if(CMPx->CMP_OutptP12_En == ENABLE)		CMPCR1 |= CMPOE;	//允许比较结果输出到P1.2,   ENABLE,DISABLE
	if(CMPx->CMP_InvCMPO     == ENABLE)		CMPCR2 |= INVCMPO;	//比较器输出取反, 	ENABLE,DISABLE
	if(CMPx->CMP_100nsFilter == DISABLE)	CMPCR2 |= DISFLT;	//内部0.1uF滤波,  	ENABLE,DISABLE
//	u8	CMP_Polity;			//中断优先级,     PolityLow,PolityHigh
}

void CMP_HL(u8 able){
	if(able){
		CMPCR1 = PIE | ( CMPCR1 & ~NIE);
	}else{
		CMPCR1 = NIE | ( CMPCR1 & ~PIE );
	}
}




void CMP_start(u8 able){
	if(able){
		CMPCR1 |= CMPEN;
	}else{
		CMPCR1 &= ~CMPEN;
	}
}
