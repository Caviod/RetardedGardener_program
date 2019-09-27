#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_can.h"      //CAN
#include  "MK60_sdhc.h"     //SDHC
#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY


#include "CL_FUN.h"
#include "pid.h"
#include "CAN_Control.h"
#include "LCD12864.h"
#include   "UART.h"
#include "remote.h"
#include "chassis.h"
#include "PIT.h"
#include "Control.h"
#include "MPU6050Drive.h"
#include "My_I2C.h"
#include "pushrod.h"
#include "main.h"
#include "Task.h"
#include "electromagnet.h"
#include "pushrod.h"
#include "EQTask.h"

#include "gyrostat.h"
#include "version.h"

#include "GetChessLocation.h"
#include "chassis_get_control.h"
#include "Butten.h"
#endif  //__INCLUDE_H__
