#ifndef __PWM_H__
#define __PWM_H__
//==============================================================================
//---------------------------------Includes-------------------------------------
//==============================================================================
#include "sys.h"


//==============================================================================
//---------------------------------Defines--------------------------------------
//==============================================================================

#define PWM_FREQ_32KHZ		255




//==============================================================================
//--------------------------------FUNCTIONS-------------------------------------
//==============================================================================
void PwmInit(u8 freq);
void PwmSetDuty(u16 duty);


//==============================================================================
//---------------------------------END FILE-------------------------------------
//==============================================================================

#endif