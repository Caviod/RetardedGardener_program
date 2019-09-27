/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           Keyboard.c
  *     @brief          矩阵键盘
  *     @note           
  *     @history             
  *  Version    Date            Author          Modification
  *  V1.0.0     2019.6.4       min              Finish
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#include "keyboard.h"

void key_board_init(void);      //初始化矩阵键盘
uint8 key_board_getval(void);

/**
* @brief                初始化矩阵键盘
* @author               闵启玄
* @retval               空
*/
void key_board_init()
{
  gpio_init(R1,GPO,0);
  gpio_init(R2,GPO,0);
  gpio_init(R3,GPO,0);  
  gpio_init(R4,GPO,0);
  
  gpio_init(C1,GPI,1);
  gpio_init(C2,GPI,1);
  gpio_init(C3,GPI,1);
  gpio_init(C4,GPI,1);
  
  port_init_NoALT(R1, PULLUP);
  port_init_NoALT(R2, PULLUP);
  port_init_NoALT(R3, PULLUP);
  port_init_NoALT(R4, PULLUP);
  port_init_NoALT(C1, PULLUP);
  port_init_NoALT(C2, PULLUP);
  port_init_NoALT(C3, PULLUP);
  port_init_NoALT(C4, PULLUP);
}

/**
* @brief                获取矩阵键盘值
* @author               闵启玄
* @retval               矩阵键盘按键值
*/
uint8 key_board_getval()
{
  uint8 r1,r2,r3,r4,c1,c2,c3,c4;
  uint8 keyvalue;
  
  gpio_ddr(R1,GPO);
  gpio_ddr(R2,GPO);
  gpio_ddr(R3,GPO);
  gpio_ddr(R4,GPO);
  
  gpio_ddr(C1,GPI);
  gpio_ddr(C2,GPI);
  gpio_ddr(C3,GPI);
  gpio_ddr(C4,GPI);
  
  gpio_set(R1,0);
  gpio_set(R2,0);
  gpio_set(R3,0);
  gpio_set(R4,0);
  
  c1=gpio_get(C1);
  c2=gpio_get(C2);
  c3=gpio_get(C3);
  c4=gpio_get(C4);
  
  if(c1&&c2&&c3&&c4 != 0)       //如果四个列没有检测到低电平
  {
    keyvalue=0;
  }
  else if(c1==0)
  {
    gpio_ddr(R1,GPI);
    gpio_ddr(R2,GPI);
    gpio_ddr(R3,GPI);
    gpio_ddr(R4,GPI);
    
    gpio_ddr(C1,GPO);
    gpio_ddr(C2,GPO);
    gpio_ddr(C3,GPO);
    gpio_ddr(C4,GPO);
    
    gpio_set(C1,0);
    gpio_set(C2,0);
    gpio_set(C3,0);
    gpio_set(C4,0);
    
    r1=gpio_get(R1);
    r2=gpio_get(R2);
    r3=gpio_get(R3);
    r4=gpio_get(R4);
    
    if(r1==0)
    {
      keyvalue= 1;
    }
    else if(r2==0)
    {
      keyvalue= 5;
    }
    else if(r3==0)
    {
      keyvalue= 9;
    }
    else if(r4==0)
    {
      keyvalue= 13;
    }
  }
  else if(c2==0)
  {
    gpio_ddr(R1,GPI);
    gpio_ddr(R2,GPI);
    gpio_ddr(R3,GPI);
    gpio_ddr(R4,GPI);
    
    gpio_ddr(C1,GPO);
    gpio_ddr(C2,GPO);
    gpio_ddr(C3,GPO);
    gpio_ddr(C4,GPO);
    
    gpio_set(C1,0);
    gpio_set(C2,0);
    gpio_set(C3,0);
    gpio_set(C4,0);
    
    r1=gpio_get(R1);
    r2=gpio_get(R2);
    r3=gpio_get(R3);
    r4=gpio_get(R4);
    
    if(r1==0)
    {
      keyvalue= 2;
    }
    else if(r2==0)
    {
      keyvalue= 6;
    }
    else if(r3==0)
    {
      keyvalue= 10;
    }
    else if(r4==0)
    {
      keyvalue= 14;
    }
  }
  else if(c3==0)
  {
    gpio_ddr(R1,GPI);
    gpio_ddr(R2,GPI);
    gpio_ddr(R3,GPI);
    gpio_ddr(R4,GPI);
    
    gpio_ddr(C1,GPO);
    gpio_ddr(C2,GPO);
    gpio_ddr(C3,GPO);
    gpio_ddr(C4,GPO);
    
    gpio_set(C1,0);
    gpio_set(C2,0);
    gpio_set(C3,0);
    gpio_set(C4,0);
    
    r1=gpio_get(R1);
    r2=gpio_get(R2);
    r3=gpio_get(R3);
    r4=gpio_get(R4);
    
    if(r1==0)
    {
      keyvalue= 3;
    }
    else if(r2==0)
    {
      keyvalue= 7;
    }
    else if(r3==0)
    {
      keyvalue= 11;
    }
    else if(r4==0)
    {
      keyvalue= 15;
    } 
  }
  else if(c4==0)
  {
    gpio_ddr(R1,GPI);
    gpio_ddr(R2,GPI);
    gpio_ddr(R3,GPI);
    gpio_ddr(R4,GPI);
    
    gpio_ddr(C1,GPO);
    gpio_ddr(C2,GPO);
    gpio_ddr(C3,GPO);
    gpio_ddr(C4,GPO);
    
    gpio_set(C1,0);
    gpio_set(C2,0);
    gpio_set(C3,0);
    gpio_set(C4,0);
    
    r1=gpio_get(R1);
    r2=gpio_get(R2);
    r3=gpio_get(R3);
    r4=gpio_get(R4);
    
    if(r1==0)
    {
      keyvalue= 4;
    }
    else if(r2==0)
    {
      keyvalue= 8;
    }
    else if(r3==0)
    {
      keyvalue= 12;
    }
    else if(r4==0)
    {
      keyvalue= 16;
    }
  }
  return keyvalue;
}

