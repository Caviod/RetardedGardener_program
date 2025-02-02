/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		FUN
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/



#ifndef _SEEKFREE_FUN_h
#define _SEEKFREE_FUN_h

#include "include.h"


int   myabs(int dat);
void  my_delay(long t);
float limit(float x, uint16 y);
int Round(float fdx);                                  //四舍五入函数
int Least_Square(int x1,int x2,int x3,int x4,int x5);  //最小二乘法函数
int16 limit_ab(int16 x, int16 a, int16 b);
extern int16 int16_range_protect( int16  duty, int16  min, int16  max ); //限幅保护








#endif
