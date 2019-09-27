#include "include.h"
#include "PIT.h"

void SwitchMode();
extern unsigned char TaskIndex;//任务进程序列
extern _EQ_Task EQ_TaskPool[60];//任务池
void PIT_INIT()
{
       pit_init_ms(PIT0, 10);                                       //初始化PIT0（10ms主控定时器）
       set_vector_handler(PIT0_VECTORn, PIT0_IRQHandler);          //把PIT0放入中断向量表
       enable_irq (PIT0_IRQn);
       
       pit_init_ms(PIT1, 10);                                       //初始化PIT1（10ms航姿推测定时器）
       set_vector_handler(PIT1_VECTORn, PIT1_IRQHandler);          //把PIT1放入中断向量表
       Dead_Recking_Reset(0, 0, 0);
       enable_irq (PIT1_IRQn);
}



SwitchCheckStatus NowSwitchCheck[4];
unsigned char NowSwitchStatus[4];
unsigned char SwitchCount[4];
unsigned long TimeMeassured = 0;
unsigned long Time_PIT1 = 0;
extern unsigned char Flag_SendIMUData;
//主控定时器
void PIT0_IRQHandler()//1.7ms
{
       PIT_Flag_Clear(PIT0);
       pit_time_start(PIT3);
        Flag_SendIMUData = 1;       
       if(Flag_RemoteStopCar == 1)
	      chassis(Chassis_Stable,0);
       else if(Flag_RemoteStopCar == 0)
       {
           //DoTask();
	      chassis(DA_Loop, 0);
       }
       GetRemoteCMDData();//1us

       HoldAndDownForTimer();

       NowSwitchStatus[0] = gpio_get(PTE1);
       NowSwitchStatus[1] = gpio_get(PTE2);
       NowSwitchStatus[2] = gpio_get(PTE3);
       NowSwitchStatus[3] = gpio_get(PTE6);
       

       for (int i = 0; i < 4; i++)
       {
	      if (NowSwitchCheck[i] == None)
	      {
		     if (!NowSwitchStatus[i])
		     {
			    NowSwitchCheck[i] = Wait;
		     }
	      }
	      if (NowSwitchCheck[i] == Wait)
	      {
		     if (!NowSwitchStatus[i])
		     {
			    SwitchCount[i]++;
		     }
		     else
		     {
			    SwitchCount[i] = 0;
			    NowSwitchCheck[i] = None;
		     }
		     if (SwitchCount[i]>1)
		     {
			    SwitchCount[i] = 0;
			    NowSwitchCheck[i] = Pressed;
		     }
	      }
	      if (NowSwitchCheck[i] == Pressed)
	      {
		     if (NowSwitchStatus[i])
		     {
			    NowSwitchCheck[i] = PressedOnce_Up;
		     }
	      }
       }
       SwitchMode();
       Check_Key();
       TimeMeassured = pit_time_get_us(PIT3);
}

//接收遥控器的目标值+计算pid并输出
void PIT1_IRQHandler()//2.5ms
{
       PIT_Flag_Clear(PIT1);
//       pit_time_start(PIT2);
       Dead_Recking();
//       Time_PIT1 = pit_time_get_us(PIT2);
}

void PIT2_IRQHandler()
{
    PIT_Flag_Clear(PIT2);
}

void PIT3_IRQHandler()
{
  PIT_Flag_Clear(PIT3);

}

void SwitchMode()
{
    if(NowSwitchCheck[0] == PressedOnce_Up)
    {
        NowSwitchCheck[0] = None;
        StartHoldChess();
        Flag_Holding = 1;
    }
    else if (NowSwitchCheck[1] == PressedOnce_Up)
    {
        NowSwitchCheck[1] = None;
        StartDownChess();
        Flag_Downing = 1;
    }
    else if (NowSwitchCheck[2] == PressedOnce_Up)
    {
        NowSwitchCheck[2] = None;
        NowMode = Chassis_Remote;
    }
    else if (NowSwitchCheck[3] == PressedOnce_Up)
    {
        NowSwitchCheck[3] = None;
        NowMode = Chassis_Stable;
    }
}
