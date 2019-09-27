/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           queen.h
  *     @brief          八皇后任务的头文件
  *     @note           
  *     @history
  *  Version    Date            Author          Modification
  *  V0.9.6     2019.6.26       min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#ifndef __QUEEN_H
#define __QUEEN_H

#include "include.h"

//此次枚举为八皇后主任务
enum
{
  Get_Queen_Location=0,//获取皇后的坐标
  Go_To_Chessboard,//前往棋盘
  Queen_Process,//八皇后操作

  Total_Task_Amount                //总的主任务数量
};

//此次枚举为棋子移动任务
enum
{
  Go_To_Queen=0,//前往皇棋
  Pick_Queen,//抓起皇后棋
  Go_To_Destination,//前往目标区域
  Put_Queen,//放置皇后棋
  
  Total_Process_Task_Amount           //总的下棋任务步骤
};

//此结构体定定义八皇后任务的变量
typedef struct __Queen
{
  uint8 Queen_Loaction[8][2];//八皇后八个棋子位置以及目标位置
  
  
}QueenTypeDef;

#endif