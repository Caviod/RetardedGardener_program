/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           pushrod.h
  *     @brief          推杆库
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V0.9.6     2019.6.30       min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#ifndef __PUSHROD_H
#define __PUSHROD_H



#define rod_static   rod_control(0)
#define rod_down   rod_control(1)
#define rod_up   rod_control(2)

#define Pushrod_CH0  FTM_CH2
#define Pushrod_CH1  FTM_CH3

#define Pushrod_Frequency 10000

#define Pushrod_OFF_Duty 0
#define Pushrod_ON_Duty 4500

extern void pushrod_init();
extern void rod_control(int8_t ud);


#endif