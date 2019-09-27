#include "include.h"

Enum_HoldAndDownChessStatus  HoldAndDownStatus = Moving_NotHaveChess;

unsigned int PushingCount_5ms = 0;
unsigned int PullingCount_5ms = 0;
///5ms定时器
void HoldAndDownForTimer_TEMP()
{
       if (HoldAndDownStatus == HoldChess_Electromagnet_On)
       {
	      electromagnet_on(UsedIndex_Electromagnet);
	      rod_down;
	      HoldAndDownStatus = HoldChess_PushrodPushing;
       }
       else if (HoldAndDownStatus == DownChess_Electromagnet_Off)
       {
	      electromagnet_off(UsedIndex_Electromagnet);
	      rod_up;
	      HoldAndDownStatus = DownChess_PushrodPulling;
       }


       if (HoldAndDownStatus == HoldChess_PushrodPushing)
       {
	      PushingCount_5ms++;
	      if (PushingCount_5ms >= PushingTime_5ms)
	      {
		     rod_up;
		     HoldAndDownStatus = HoldChess_PushrodPulling;
		     PushingCount_5ms = 0;
	      }
       }
       else if (HoldAndDownStatus == HoldChess_PushrodPulling)
       {
	      PullingCount_5ms++;
	      if (PullingCount_5ms >= PullingTime_5ms)
	      {
		     rod_static;
		     HoldAndDownStatus = Moving_HaveChess;
		     PullingCount_5ms = 0;
	      }
       }
       else if (HoldAndDownStatus == DownChess_PushrodPushing)
       {
	      PushingCount_5ms++;
	      if (PushingCount_5ms >= PushingTime_5ms)
	      {
		     HoldAndDownStatus = DownChess_Electromagnet_Off;
		     PushingCount_5ms = 0;
	      }
       }
       else if (HoldAndDownStatus == DownChess_PushrodPulling)
       {
	      PullingCount_5ms++;
	      if (PullingCount_5ms >= PullingTime_5ms)
	      {
		     rod_static;
		     HoldAndDownStatus = Moving_NotHaveChess;
		     PullingCount_5ms = 0;
	      }
       }
}


int  HoldChess_count=0;
int  PlaceChess_count=0;
void HoldAndDownForTimer1()
{
       switch(HoldAndDownStatus)
       {
	    case HoldChess_Electromagnet_On:
	      {

		     switch(HoldChess_count)
		     {
			  case 0:
			    {
				   electromagnet_on(UsedIndex_Electromagnet);
				   rod_down;
				   //HoldAndDownStatus = HoldChess_PushrodPushing;
				   HoldChess_count=1;
				   break;
			    }
			  case 1:
			    {
				   PushingCount_5ms++;
				   if (PushingCount_5ms >= PushingTime_5ms)
				   {
					  rod_up;
					  // HoldAndDownStatus = HoldChess_PushrodPulling;
					  PushingCount_5ms = 0;
					  HoldChess_count=2;
				   }
				   break;
			    }
			  case 2:
			    {
				   PullingCount_5ms++;
				   if (PullingCount_5ms >= PullingTime_5ms)
				   {
					  rod_static;
					  HoldAndDownStatus = Moving_HaveChess;
					  PullingCount_5ms = 0;
				   }
				   break;
			    }
		     }
		     break;
	      }
	    case DownChess_PushrodPushing:
	      {
		     switch(PlaceChess_count)
		     {
			  case 0:
			    {
				   rod_down;
				   PlaceChess_count=1;
				   break;
			    }
			  case 1:
			    {
				   PushingCount_5ms++;

				   if (PushingCount_5ms >= 80)
				   {
					 electromagnet_off(UsedIndex_Electromagnet);
				   }
				   if (PushingCount_5ms >= PushingTime_5ms)
				   {
					  rod_up;
					  PushingCount_5ms = 0;
					  PlaceChess_count=2;
				   }
				   break;
			    }
			  case 2:
			    {
				   PullingCount_5ms++;
				   if (PullingCount_5ms >= PullingTime_5ms)
				   {
					  rod_static;
					  HoldAndDownStatus = Moving_NotHaveChess;
					  PullingCount_5ms = 0;
				   }
				   break;
			    }
		     }
		     break;
	      }
       }
}

void HoldAndDownForTimer()
{
       switch(HoldAndDownStatus)
       {
	    case HoldChess_Electromagnet_On:
	      {

		     switch(HoldChess_count)
		     {
			  case 0:
			    {
				   electromagnet_on(UsedIndex_Electromagnet);
				   rod_down;
				   //HoldAndDownStatus = HoldChess_PushrodPushing;
				   HoldChess_count=1;
				   break;
			    }
			  case 1:
			    {
				   PushingCount_5ms++;
				   if (PushingCount_5ms >= PushingTime_5ms)
				   {
					  rod_up;
					  // HoldAndDownStatus = HoldChess_PushrodPulling;
					  PushingCount_5ms = 0;
					  HoldChess_count=2;
				   }
				   break;
			    }
			  case 2:
			    {
				   PullingCount_5ms++;
				   if (PullingCount_5ms >= PullingTime_5ms)
				   {
					  rod_static;
					  HoldAndDownStatus = Moving_HaveChess;
					  PullingCount_5ms = 0;
				   }
				   break;
			    }
		     }
		     break;
	      }
	    case DownChess_PushrodPushing:
	      {
		     switch(PlaceChess_count)
		     {
			  case 0:
			    {
				   PlaceChess_count=1;
				   break;
			    }
			  case 1:
			    {
				   PushingCount_5ms++;
				   if (PushingCount_5ms >= 0)
				   {
					  electromagnet_off(UsedIndex_Electromagnet);
					  PushingCount_5ms = 0;
					  PlaceChess_count=2;
				   }
				   break;
			    }
			  case 2:
			    {
				   PullingCount_5ms++;
				   if (PullingCount_5ms >= 50)
				   {
					  rod_static;
					  HoldAndDownStatus = Moving_NotHaveChess;
					  PullingCount_5ms = 0;
				   }
				   break;
			    }
		     }
		     break;
	      }
       }
}

unsigned char StartHoldChess()
{
    if(HoldAndDownStatus==Moving_NotHaveChess)
    {
        HoldAndDownStatus = HoldChess_Electromagnet_On;
	HoldChess_count=0;
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char StartDownChess()
{
    if (HoldAndDownStatus == Moving_HaveChess)
    {
        HoldAndDownStatus = DownChess_PushrodPushing;
	PlaceChess_count=0;
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char Check_HoldOrDownFinish()
{
    if (HoldAndDownStatus == Moving_NotHaveChess || HoldAndDownStatus == Moving_HaveChess)
        return 1;
    else
        return 0;
}


extern uint8 HLineLocation_Right;
# define HLineJumpThreshold 54//横线跳变数格子阈值
uint8 LastHLine_Y_Front = 50;//上次横线Y坐标
uint8 LastHLine_Y_Right = 50;
int GridNum_Front = 0;//前进的格子数
int GridNum_Right = 0;//前进的格子数
///数格子函数
#if 0
void CountGrid()
{
  if(HLineLocation - LastHLine_Y_Front > HLineJumpThreshold)//检测上升沿跳变,代表向前走了一格
  {
    GridNum_Front--;
  }
  else if(HLineLocation - LastHLine_Y_Front < -HLineJumpThreshold)//检测下降沿跳变
  {
    GridNum_Front++;
  }
  if(HLineLocation_Right - LastHLine_Y_Right > HLineJumpThreshold)//检测上升沿跳变,代表向前走了一格
  {
    GridNum_Right--;
  }
  else if(HLineLocation_Right - LastHLine_Y_Right < -HLineJumpThreshold)//检测下降沿跳变
  {
    GridNum_Right++;
  }
  LastHLine_Y_Front = HLineLocation;
  LastHLine_Y_Right = HLineLocation_Right;
}
#endif