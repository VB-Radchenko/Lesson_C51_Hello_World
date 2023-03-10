#ifndef __ADC_H__
#define __ADC_H__

//==============================================================================
//---------------------------------Includes-------------------------------------
//==============================================================================
#include "sys.h"

//==============================================================================
//---------------------------------Defines--------------------------------------
//==============================================================================

//==============================================================================
//-----------------------------------ENUM---------------------------------------
//==============================================================================
enum {
	ADC_CHANNEL0 = 0x00,
	ADC_CHANNEL1,
	ADC_CHANNEL2,
	ADC_CHANNEL3,
	ADC_CHANNEL4,
	ADC_CHANNEL5,
	ADC_CHANNEL6,
	ADC_CHANNEL7
};

//==============================================================================
//--------------------------------FUNCTIONS-------------------------------------
//==============================================================================
u16 AdcRead(u16 channel);
u16 AdcReadAvg(u16 channel,u16 avg_cnt);

//==============================================================================
//---------------------------------END FILE-------------------------------------
//==============================================================================
#endif