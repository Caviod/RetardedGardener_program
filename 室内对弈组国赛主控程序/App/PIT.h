#ifndef __PID_H
#define __PID_H

#include "include.h"

extern void PIT_INIT();
extern void PIT0_IRQHandler();
extern void PIT1_IRQHandler();
extern void PIT2_IRQHandler();
extern void PIT3_IRQHandler();
typedef enum
{
    None,
    Wait,
    Pressed,
    PressedOnce_Up
}SwitchCheckStatus;

extern SwitchCheckStatus NowSwitchCheck[4];


#endif