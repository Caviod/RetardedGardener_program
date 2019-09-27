/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           electromagnet.h
  *     @brief          电磁铁库
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V0.9.6     2019.6.28       min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#ifndef __ELECTROMAGNET_H
#define __ELECTROMAGNET_H



#define Electromagnet_CH1     FTM_CH0
#define Electromagnet_CH2     FTM_CH1
#define Electromagnet_CH3     FTM_CH0
#define Electromagnet_CH4     FTM_CH1

#define Electromagnet_FTM1      FTM2
#define Electromagnet_FTM2      FTM2
#define Electromagnet_FTM3      FTM1
#define Electromagnet_FTM4      FTM1

#define Electromagnet_Frequency 1000
#define Electromagnet_On_Duty   6000
#define Electromagnet_Off_Duty  0

enum __Electromagnet_Num
{
  Electromagnet1,
  Electromagnet2,
  Electromagnet3,
  Electromagnet4
};

extern void electromagnet_init();
extern void electromagnet_on(uint8 Electromagnet_Number);
extern void electromagnet_off(uint8 Electromagnet_Number);

#endif