#ifndef __BUTTEN_H
#define __BUTTEN_H

#include "include.h"

typedef enum
{
    No_Pressed,
    Wait_Pressed,
    Be_Pressed,
    Short_Pressed,
    Mid_Pressed,
    Long_Pressed,
}CheckStatus;
extern void Check_Key();
extern void Switch_Action();


typedef enum
{
  Doing = 0,
  DoNextPolicy,
  DoLastPolicy  
}PolicyMode;
extern PolicyMode RunPolicyMode;

typedef enum
{
  Fast,
  Slow,
  LightOff
}RunModeLedHintStatus;
extern RunModeLedHintStatus LEDHintStatus;

extern void LEDHintStart(RunModeLedHintStatus DoStatus);
extern void LedHintForTimer();//10ms
extern void RenewRunMode();

#endif