#include	"config.h"
#include	"adc.h"
#include	"delay.h"
#include	"soft_uart.h"
#include	"compare.h"


/*************	功能说明	**************

本程序演示多路ADC查询采样，通过模拟串口发送给上位机，波特率9600,8,n,1。
用户可以修改为1~8路的ADC转换。

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/


void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				                    //结构定义
	ADC_InitStructure.ADC_Px        = ADC_P11;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			                //ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			                  //ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		              //ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		                //优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = ENABLE;			                  //中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					                      //初始化
	ADC_PowerControl(ENABLE);							                          //单独的ADC电源操作函数, ENABLE或DISABLE
}


/************************ 比较器配置 ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//结构定义
	CMP_InitStructure.CMP_EN = ENABLE;					//允许比较器		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = ENABLE;	//允许上升沿中断	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = ENABLE;	//允许下降沿中断	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_ADCIS;		//比较器输入正极性选择, CMP_P_P55: 选择内部P5.5做正输入, CMP_P_ADCIS: 由ADCIS[2:0]所选择的ADC输入端做正输入.
	CMP_InitStructure.CMP_N_Select     = CMP_N_P54;		//比较器输入负极性选择, CMP_N_BGv: 选择内部BandGap电压BGv做负输入, CMP_N_P54: 选择外部P5.4做输入.
	CMP_InitStructure.CMP_OutptP12_En  = DISABLE;		//允许比较结果输出到P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//比较器输出取反, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;		//内部0.1uF滤波,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 0;			//比较结果变化延时周期数, 0~63
//	CMP_InitStructure.CMP_Polity	   = PolityHigh;	//中断优先级,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2

	CMPCR1 &= ~CMPIF;

}


/**********************************************/
void main(void)
{
	u8	i;
	u16	j;
	
	ADC_config();
	adcRequest(ADC_P11);
	
	EA = 1;
	while (1)
	{
		delay_ms(2000);
		PrintString("test");
	}
}

//========================================================================
// 函数: void ADC_int(void) interrupt ADC_VECTOR
// 描述: ADC中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2012-10-22
//========================================================================
void ADC_int (void) interrupt ADC_VECTOR
{
	//清除标志
	ADC_CONTR &= ~ADC_FLAG;
	adcRequest(ADC_P11);
}



/********************* UART1中断函数************************/
void CMP_int (void) interrupt CMP_VECTOR
{
	CMPCR1 &= ~CMPIF;	//清除中断标志
	PrintString("CMP_int");
	
}








