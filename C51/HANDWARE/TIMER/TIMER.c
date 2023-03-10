//==============================================================================
//---------------------------------Includes-------------------------------------
//==============================================================================
#include "TIMER.h"

//==============================================================================
//---------------------------------Defines--------------------------------------
//==============================================================================
#define T0_PERIOD_1MS		0xBCCD		//1ms timing period
xdata u16 TimeOut = 0;
xdata u32 Counter = 1000;

//==============================================================================
//---------------------------------- INIT---------------------------------------
//==============================================================================
void TimerInit()
{
	//16-bit timer working mode
	TMOD &= 0xFC;
	TMOD |= 0x01;
	//Timing period is 1ms
	TH0 = (u8)(T0_PERIOD_1MS>>8); 
	TL0 = (u8)T0_PERIOD_1MS;
	ET0 = 1;//Enable timer 0 Interrupt
	EA = 1;//Enable total interrupt
	TR0 = 1;//Start timer 0
}

//==============================================================================
//--------------------------------INTERRUPT-------------------------------------
//==============================================================================
void timer0_isr()	interrupt 1
{
	static idata u32 CntTimer = 0;
	//Reload period value
	TH0 = (u8)(T0_PERIOD_1MS>>8); 
	TL0 = (u8)T0_PERIOD_1MS;
	CntTimer++;
	if(CntTimer==Counter)//1s timing period is up
	{
		CntTimer = 0;
		TimeOut = 1;//Perform an operation that increments by 1
	}	
}


//==============================================================================
//--------------------------------FUNCTIONS-------------------------------------
//==============================================================================
void StartTimer(u32 time)
{
	Counter = time;
}

u16 GetTimeOutFlag()
{
	return TimeOut;
}
void ClearTimeFlag()
{
	TimeOut = 0;
}

//==============================================================================
//---------------------------------END FILE-------------------------------------
//==============================================================================
