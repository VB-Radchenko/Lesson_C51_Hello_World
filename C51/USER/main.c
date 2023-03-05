
//==============================================================================
//---------------------------------Includes-------------------------------------
//==============================================================================
#include "sys.h"
#include "ADC.h"
#include "UART.h"
#include "TIMER.h"
#include "GPIO.h"
//==============================================================================
//---------------------------------Defines--------------------------------------
//==============================================================================
#define ADR_CNT					0x5000
#define ADR_ADC					0x5010
#define ADR_LED					0x5020
#define ADR_BUTTON			0x5030


sbit GPIO_LED_0 = P1^0;
sbit GPIO_LED_1 = P1^1;
sbit GPIO_LED_2 = P1^2;
sbit GPIO_BUTTON = P1^3; 

//==============================================================================
//--------------------------------Variables-------------------------------------
//==============================================================================

xdata u16 CounterVar = 0;
u16 ButState = 0;
u16 ReadU16 = 0;

//==============================================================================
//--------------------------------PROTOTYPE-------------------------------------
//==============================================================================

//==============================================================================
//---------------------------------- INIT---------------------------------------
//==============================================================================

void GpioInit()
{
	SetPinOut(GPIO_PORT_1,GPIO_PIN_0);
	SetPinOut(GPIO_PORT_1,GPIO_PIN_1);
	SetPinOut(GPIO_PORT_1,GPIO_PIN_2);
	SetPinIn(GPIO_PORT_1,GPIO_PIN_3);
}


void MainInit()
{
	InitCPU();		//System initialization
	GpioInit();		//GPIO pin initialization
	TimerInit();  //Timer initialization
	StartTimer(1000);
	GPIO_LED_0 = GPIO_OUT_HIGHT;
	GPIO_LED_1 = GPIO_OUT_LOW;
	GPIO_LED_2 = GPIO_OUT_LOW;
	CounterVar = 0;
	WriteDgusVp(ADR_CNT,(u8*)&CounterVar,1);
}



//==============================================================================
//--------------------------------FUNCTIONS-------------------------------------
//==============================================================================
void GpioProcess()
{
	ReadU16 = 0;
	ReadDgusVp(ADR_LED,(u8*)&ReadU16,1);
	ButState = GPIO_BUTTON;
	
	if (ReadU16 == 0)
	{
		GPIO_LED_0 = GPIO_OUT_HIGHT;
	}
	else
	{
		GPIO_LED_0 = GPIO_OUT_LOW;
	}
	
	if (ButState == 0)
	{
		u16 writeVar = 1;
		GPIO_LED_1 = GPIO_OUT_HIGHT;
		WriteDgusVp(ADR_BUTTON,(u8*)&writeVar,1);
	}
	else
	{
		u16 writeVar = 0;
		GPIO_LED_1 = GPIO_OUT_LOW;
		WriteDgusVp(ADR_BUTTON,(u8*)&writeVar,1);
		
	}
}

void AdcProcess()
{
	u16 AdcData =  AdcReadAvg(ADC_CHANNEL1,100);
	WriteDgusVp(ADR_ADC,(u8*)&AdcData,1);
}


void CounterProcess()
{
	if (GetTimeOutFlag())
	{
		ClearTimeFlag();
		CounterVar++;
		GPIO_LED_2 = ~GPIO_LED_2;
		WriteDgusVp(ADR_CNT,(u8*)&CounterVar,1);
	}
}


//==============================================================================
//-----------------------------------MAIN---------------------------------------
//==============================================================================
void main(void)
{   
	
	MainInit();
	while(1)
	{ 
		GpioProcess();	
    AdcProcess();	
    CounterProcess();		
	}
}


//==============================================================================
//---------------------------------END FILE-------------------------------------
//==============================================================================
