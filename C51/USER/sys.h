#ifndef __SYS_H__
#define __SYS_H__
//==============================================================================
//---------------------------------Includes-------------------------------------
//==============================================================================
#include "t5los8051.h"

//==============================================================================
//---------------------------------Typedef--------------------------------------
//==============================================================================


//类型重定义
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;
typedef signed char     s8;
typedef signed short    s16;
typedef signed long     s32;


//==============================================================================
//---------------------------------Defines--------------------------------------
//==============================================================================
#define		NOR_FLASH					0x0008
#define		SOFT_VERSION			0x000F
#define		RTC								0x0010
#define		PIC_NOW						0x0014
#define		TP_STATUS					0x0016
#define		LED_NOW						0x0031
#define		AD_VALUE					0x0032
#define		SYSTEM_CONFIG			0x0080
#define		LED_CONFIG				0x0082
#define		PIC_SET						0x0084
#define 	RTC_Set						0x009C	





#define	WDT_ON()				MUX_SEL|=0x02		//开启看门狗
#define	WDT_OFF()				MUX_SEL&=0xFD		//关闭看门狗
#define	WDT_RST()				MUX_SEL|=0x01		//喂狗

//系统主频和1ms定时数值定义
#define FOSC     				206438400UL
#define T1MS    				(65536-FOSC/12/1000)

 



//==============================================================================
//--------------------------------FUNCTIONS-------------------------------------
//==============================================================================
void SysDelayMs(u16 ms);
void SysDelayUs(u16 us);


u16 ReadDgus(u16 Dgus_Addr);
void WriteDgus(u16 Dgus_Addr,u16 Val);
void WriteDgusVp(u16 Addr,u8* pBuf,u8 Len16);
void ReadDgusVp(u16 Addr,u8* pBuf,u8 Len16);
/*init*/
void InitCPU(void);
u16 GetPageID();
void Page_Change(u16 PageID);
//==============================================================================
//---------------------------------END FILE-------------------------------------
//==============================================================================

#endif


