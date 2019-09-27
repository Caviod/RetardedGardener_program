/**
  ****************************(C) COPYRIGHT 2019 SMU智能车实验室****************************
  *     @file           remote.h
  *     @brief          遥控库头文件
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2019.6.10      陈云帆                Finish
  ****************************(C) COPYRIGHT 2019 SMU智能车实验室****************************
  */
#ifndef __REMOTE_H
#define __REMOTE_H

#include "include.h"
extern void initialization();
extern void get_speed();
extern void get_XYZ(int X,int Y, int Z);
extern void get_TargetSpeed();
extern void calculate_error();
extern void calculate_PID_output();
extern void calculate_output();
extern void get_lasttargetspeed();
extern void OpenMVCollectInit();

//四轮输出
extern int16 output[4];
extern int16 Now_Speed[4];      //当前can发送过来的四个轮子的速度 0左前轮 1右前轮  2右后轮 3左后轮
extern int16 TargetSpeed[4];    //四轮目标值
extern int16 NowError[4];       //四轮当前偏差
extern int16 LastError[4];      //四轮上次偏差
extern int16 PrevError[4];      //四轮上上次偏差

extern int X_Speed,Y_Speed,Z_Speed;




# define Remote_Uart_Port UART1//0
# define Collect_Uart_Port UART5//0
# define OpenMVCollect_Uart_Port UART3//0

typedef enum
{
  Left_Return0,
  Left_Left,
  Left_Right,
  Left_Up,
  Left_Down,
  Right_Return0,
  Right_Left,
  Right_Right,
  Right_Up,
  Right_Down,
  Start
}RemoteCMDMode;

/// <summary>
///接受遥控指令程序，应放入对应的串口中断内
///</summary>
void ReceiveCMD_Remote();
/// <summary>
///初始化遥控器的串口
///</summary>
void RemoteInit();


#ifndef Remote_UseDigitalReceive
typedef enum
{
    Sleep,
    SendLeftCMD,
    ReceivingLeftCMD,
    ReceivedLeftCMD,
    SendRightCMD,
    ReceivingRightCMD,
    ReceivedRightCMD
}Remote_State;


typedef struct
{
    uint8 Left_X;
    uint8 Left_Y;
    uint8 Right_X;
    uint8 Right_Y;
}ReceiveCMDData;
void GetRemoteCMDData(void);

#endif

extern unsigned char Flag_RemoteStopCar;

void SetSpeed_FromRemote_Analog(void);
extern ReceiveCMDData RemoteData;
extern void Chassist_init(ReceiveCMDData *Initial_Remote);
#endif