/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           destination.c
  *     @brief          目标任务
  *     @note           完成目标是定点任务封装成函数方便进行调试
  *     @history
  *  Version    Date            Author          Modification
  *  V0.9.6     2019.6.26       min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#include "destination.h"
/*函数定义*/
void destination(uint8 Destination);
void destination_init(void);//定点初始化
void car_position_update(PositionTypeDef *Position_t);//更新当前车位置信息
void destination_calculate(PositionTypeDef *Position_t,uint8 Destination);

/*全局变量定义*/
//棋盘数组
uint8 ChessBoard[8][8]=
{
  {1,   2,      3,      4,      5,      6,      7,      8},
  {9,   10,     11,     12,     13,     14,     15,     16},
  {17,  18,     19,     20,     21,     22,     23,     24},
  {25,  26,     27,     28,     29,     30,     31,     32},
  {33,  34,     35,     36,     37,     38,     39,     40},
  {41,  42,     43,     44,     45,     46,     47,     48},
  {49,  50,     51,     52,     53,     54,     55,     56},
  {57,  58,     59,     60,     61,     62,     63,     64}
};
//AprilTag分布数组
uint8 TagBoard[8][4]=
{
  {0,   1,      2,      3},
  {4,   5,      6,      7},
  {8,   9,      10,     11},
  {12,  13,     14,     15},
  {16,  17,     18,     19},
  {20,  21,     22,     23},
  {24,  25,     26,     27},
  {28,  29,     30,     31}
};
PositionTypeDef CarPosition;
PositionTypeDef Tag1Position;
PositionTypeDef Tag2Position;
PositionTypeDef DestinationPosition;

//定点初始化
void destination_init()
{

}

//定点任务总函数
void destination(uint8 Destination)
{
  car_position_update(&CarPosition);//跟新当前车的位置信息
  if(CarPosition.Now_Position==Destination)//如果当前位置已经到达了目标位置，则返回
    return;
  destination_calculate(&DestinationPosition,Destination);//计算目标的行和列

}

//计算目标点的位置
void destination_calculate(PositionTypeDef *Position_t,uint8 Destination)
{
  Position_t->Row_Position=Destination/4+1;//行信息是码数整除4加1
  if(Position_t->Row_Position%2==1)//行为奇数行，则列等于余数乘以二加一
    Position_t->Col_Position=Destination%4*2+1;
  else if(Position_t->Row_Position%2==0)//行为偶数行，则列数等于余数乘以二加二
    Position_t->Col_Position=Destination%4*2+2;
}
//更新车位置
void car_position_update(PositionTypeDef *Position_t)
{
  //position_calculate(&Tag1Position,&Openmv1_Msg);
 // position_calculate(&Tag2Position,&Openmv2_Msg);

  Position_t->Col_Position=Tag1Position.Col_Position;
  Position_t->Row_Position=Tag2Position.Row_Position;
  Position_t->Now_Position=ChessBoard[Position_t->Col_Position][Position_t->Row_Position];
}

//位置计算
//void position_calculate(PositionTypeDef *Position_t,AprilTag_MSG *Openmv_Msg_t)
//{
//  Position_t->Row_Position=Openmv_Msg_t->tag_id/4+1;//行信息是码数整除4加1
//  if(Position_t->Row_Position%2==1)//行为奇数行，则列等于余数乘以二加一
//    Position_t->Col_Position=Openmv_Msg_t->tag_id%4*2+1;
//  else if(Position_t->Row_Position%2==0)//行为偶数行，则列数等于余数乘以二加二
//    Position_t->Col_Position=Openmv_Msg_t->tag_id%4*2+2;
//  Position_t->Now_Position=ChessBoard[Position_t->Col_Position][Position_t->Row_Position];//导入表中获取位置信息
//}



