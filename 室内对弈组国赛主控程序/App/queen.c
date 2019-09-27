/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           queen.c
  *     @brief          八皇后任务
  *     @note           
  *     @history
  *  Version    Date            Author          Modification
  *  V0.9.6     2019.6.26       min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#include "queen.h"

/*函数定义*/
void queen(void);                       //八皇后任务的主函数
void queen_init(void);                  //八皇后任务的初始化

/*全局变量定义*/
void queen()
{
  uint8 Main_Task=0;
  switch(Main_Task)
  {
  case Get_Queen_Location:
    {
      break;
    }
  case Go_To_Chessboard:
    {
      break;
    }
  case Queen_Process:
    {
      uint8 Chess_Motion=0;
      switch(Chess_Motion)
      {
      case Go_To_Queen:
        {
          break;
        }
      case Pick_Queen:
        {
          break;
        }
      case Go_To_Destination:
        {
          break;
        }
      }
      break;
    }
  }
  
}

