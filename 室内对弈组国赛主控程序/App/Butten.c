#include "Butten.h"
#include "include.h"

CheckStatus SwitchCheck[4];
int SwitchStatus[4] = 0;
int ButtenCount[4] = 0;
int SwitchFlag[4] = 0;
unsigned long LED_Count = 0;
void Check_Key()
{
  SwitchStatus[0] = gpio_get(PTB1);
  SwitchStatus[1] = gpio_get(PTB2);
  SwitchStatus[2] = gpio_get(PTB3);
  SwitchStatus[3] = gpio_get(PTB4);
  
  for(int i=0; i<4; i++)
  {
    if(SwitchCheck[i] == No_Pressed)
    {
      led(LED0, LED_OFF);
      if(!SwitchStatus[i])
      {
        SwitchCheck[i] = Wait;
      }
    }
    if(SwitchCheck[i] == Wait)
    {
      if(!SwitchStatus[i])
      {
        ButtenCount[i]++;
      }
      else
      {
        SwitchCheck[i] = No_Pressed;
      }  
    }
  }
  if(ButtenCount[0]!= 0 && (ButtenCount[0] % 100) < 20)//17fps
  {
    led(LED0, LED_ON);
  }
  else
  {
    led(LED0, LED_OFF);
  }
  for(int i=0; i<4; i++)
  {
    if(SwitchStatus[i])
    {
      if(ButtenCount[i] > 5 && ButtenCount[i]<=300)
      {
        SwitchFlag[i] = Short_Pressed;
        ButtenCount[i] = 0;
        SwitchCheck[i] = No_Pressed;
      }
      else if((ButtenCount[i]>300) && (ButtenCount[i]<=500))
      {
        SwitchFlag[i] = Mid_Pressed;
        ButtenCount[i] = 0;
        SwitchCheck[i] = No_Pressed;
      }
      else if(ButtenCount[i]>500)
      {
        SwitchFlag[i] = Long_Pressed;
        ButtenCount[i] = 0;
        SwitchCheck[i] = No_Pressed;
      }
    }
  }
  Switch_Action();
  RenewRunMode();
}

PolicyMode RunPolicyMode = Doing;
RunModeLedHintStatus LEDHintStatus = LightOff;

void LedHintForTimer()//10ms
{
  if(LEDHintStatus == LightOff)
  {
    led(LED0, LED_OFF);
  }
  else if(LEDHintStatus == Fast)
  {
    LED_Count++;
    if((LED_Count / 3) % 2 == 0)//17fps
    {
      led(LED0, LED_OFF);
      gpio_set(PTC13, 1);
    }
    else
    {
      led(LED0, LED_ON);
      gpio_set(PTC13, 0);
    }
    if(LED_Count > 300)//30s
    {
      LED_Count = 0;
      LEDHintStatus = LightOff;
    }
  }
  else if(LEDHintStatus == Slow)
  {
    LED_Count++;
    if((LED_Count / 30) % 2 == 0)//1.7fps
    {
      led(LED0, LED_OFF);
      gpio_set(PTC13, 1);
    }
    else
    {
      led(LED0, LED_ON);
      gpio_set(PTC13, 0);
    }
    if(LED_Count > 300)//30s
    {
      LED_Count = 0;
      LEDHintStatus = LightOff;
    }
  }
}
void LEDHintStart(RunModeLedHintStatus DoStatus)
{
  LEDHintStatus = DoStatus;
}
void Switch_Action()
{
  for(int i; i<4; i++)
  {
   switch(SwitchFlag[i])
   {
   case Short_Pressed:
     {
      //SwitchFlag[i]=1;
      break;
     }
   case Mid_Pressed:
     {
      //SwitchFlag[i]=2;
      break;
     }
   case Long_Pressed:
     {
      //SwitchFlag[i]=3;
      break;
     }
    }
  }
}

void RenewRunMode()
{
  if(SwitchFlag[0] == Short_Pressed)
  {
    RunPolicyMode = DoNextPolicy;
    LEDHintStart(Slow);
    SwitchFlag[0] = No_Pressed;
  }
  else if(SwitchFlag[0] == Long_Pressed || SwitchFlag[0] == Mid_Pressed)
  {
    RunPolicyMode = DoLastPolicy;
    LEDHintStart(Fast);
    SwitchFlag[0] = No_Pressed;
  }
  else
  {
    SwitchFlag[0] = No_Pressed;
  }
}

