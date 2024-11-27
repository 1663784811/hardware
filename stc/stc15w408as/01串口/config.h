
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


#ifndef		__CONFIG_H
#define		__CONFIG_H


/*********************************************************/

//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
#define MAIN_Fosc		24000000L	//定义主时钟


/*********************************************************/
#include <STC15.H>


sfr	TH2  = 0xD6;
sfr	TL2  = 0xD7;
sfr PCON2   = 0x97;




/**********************************************************/
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;



/***************************************   interrupt vector ***************************************/
#define		INT0_VECTOR		0
#define		TIMER0_VECTOR	1
#define		INT1_VECTOR		2
#define		TIMER1_VECTOR	3
#define		UART1_VECTOR	4
#define		ADC_VECTOR		5
#define		LVD_VECTOR		6
#define		PCA_VECTOR		7
#define		UART2_VECTOR	8
#define		SPI_VECTOR		9
#define		INT2_VECTOR		10
#define		INT3_VECTOR		11
#define		TIMER2_VECTOR	12
#define		INT4_VECTOR		16
#define		UART3_VECTOR	17
#define		UART4_VECTOR	18
#define		TIMER3_VECTOR	19
#define		TIMER4_VECTOR	20
#define		CMP_VECTOR		21


#define	TRUE	1
#define	FALSE	0

//=============================================================

//========================================

#define	PolityLow			0	//低优先级中断
#define	PolityHigh			1	//高优先级中断

//========================================
#define		ENABLE		1
#define		DISABLE		0

















#endif

