#ifndef __CHASSIS_GET_CONTROL
#define __CHASSIS_GET_CONTROL

#include "include.h"
//发送陀螺仪数据
extern void Achieve_Gyro();
extern void Create_Send();
extern void Gyro_Send();
extern void PlannerError_uart_init();

//接收偏差数据
typedef struct
{
       float target_angle;
       float target_x;
       float target_y;
       uint8 next_action;
       uint8 LastElectromagnetFlag;
       uint8 ElectromagnetFlag;
} PlannerError;
extern void PlannerError_receive();
extern PlannerError PlannerError_Origin;
extern float DRx, DRy, DRw;
extern float CORRx,CORRy,CORRw;

void Dead_Recking();
void Dead_Recking_Reset(float x, float y, float angle);

extern long Delta_Time;
extern long Time_Now,Time_Last;
extern float DRx, DRy, DRw;
extern float CORRx,CORRy,CORRw;
extern int speed_max;

#endif