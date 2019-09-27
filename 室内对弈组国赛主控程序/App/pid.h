/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       pid.c/h
  * @brief      pid实现函数，包括初始化，PID计算函数，
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef PID_H
#define PID_H

#include "include.h"

enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA,
    PD_WithGYRO,
    PID_PosDel,     //位置增量融合；
    PID_chassis
};

typedef struct
{
    uint8_t mode;
    //PID 三参数
    float32_t Kp;
    float32_t Ki;
    float32_t Kd;

    float32_t max_out;  //最大输出
    float32_t max_iout; //最大积分输出

    float32_t set;
    float32_t fdb;

    float32_t integral;

    float32_t out;
    float32_t tempout;
    float32_t Pout;
    float32_t Iout;
    float32_t Dout;
    float32_t Dbuf[3];  //微分项 0最新 1上一次 2上上次
    float32_t error[3]; //误差项 0最新 1上一次 2上上次

    float32_t ExternalDifferential;//外部微分项
}PidTypeDef;

extern void PID_Init(PidTypeDef *pid, uint8_t mode, const float32_t PID[3], float32_t max_out, float32_t max_iout);
extern float32_t PID_Calc(PidTypeDef *pid, float32_t ref, float32_t set);
extern void PID_clear(PidTypeDef *pid);
#endif
