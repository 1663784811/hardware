#include "compare.h"


//========================================================================
// ����:void	CMP_Inilize(CMP_InitDefine *CMPx)
// ����: ��ʱ����ʼ������.
// ����: CMPx: �ṹ����,��ο�compare.h��Ķ���.
// ����: �ɹ�����0, �ղ�������1,���󷵻�2.
// �汾: V1.0, 2012-10-22
//========================================================================
void	CMP_Inilize(CMP_InitDefine *CMPx)
{
	CMPCR1 = 0;
	CMPCR2 = CMPx->CMP_OutDelayDuty & 0x3f;						//�ȽϽ���仯��ʱ������, 0~63
	if(CMPx->CMP_EN == ENABLE)				CMPCR1 |= CMPEN;	//����Ƚ���		ENABLE,DISABLE
	if(CMPx->CMP_RiseInterruptEn == ENABLE)	CMPCR1 |= PIE;		//�����������ж�	ENABLE,DISABLE
	if(CMPx->CMP_FallInterruptEn == ENABLE)	CMPCR1 |= NIE;		//�����½����ж�	ENABLE,DISABLE
	if(CMPx->CMP_P_Select  == CMP_P_ADCIS)	CMPCR1 |= PIS;		//�Ƚ�������������ѡ��, CMP_P_P55: ѡ���ڲ�P5.5��������, CMP_P_ADCIS: ��ADCIS[2:0]��ѡ���ADC�������������.
	if(CMPx->CMP_N_Select  == CMP_N_P54)	CMPCR1 |= NIS;		//�Ƚ������븺����ѡ��, CMP_N_BGv: ѡ���ڲ�BandGap��ѹBGv��������, CMP_N_P54: ѡ���ⲿP5.4������.
	if(CMPx->CMP_OutptP12_En == ENABLE)		CMPCR1 |= CMPOE;	//����ȽϽ�������P1.2,   ENABLE,DISABLE
	if(CMPx->CMP_InvCMPO     == ENABLE)		CMPCR2 |= INVCMPO;	//�Ƚ������ȡ��, 	ENABLE,DISABLE
	if(CMPx->CMP_100nsFilter == DISABLE)	CMPCR2 |= DISFLT;	//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
//	u8	CMP_Polity;			//�ж����ȼ�,     PolityLow,PolityHigh
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
