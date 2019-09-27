/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           electromagnet.c
  *     @brief          电磁铁库
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V0.9.6     2019.6.28       min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#include "include.h"
#include "electromagnet.h"


/*******************************************分割线***********************************************/
void electromagnet_init()
{
       gpio_init (PTA10, GPO,0);
       gpio_init (PTA11, GPO,0);
       gpio_init (PTA12, GPO,0);
       gpio_init (PTA13, GPO,0);
}


void electromagnet_on(uint8 Electromagnet_Number) //开
{
       switch(Electromagnet_Number)
       {
	    case Electromagnet1:
	      gpio_set(PTA10, 1);
	      break;
	    case Electromagnet2:
	      gpio_set(PTA11, 1);
	      break;
	    case Electromagnet3:
	      gpio_set(PTA12, 1);
	      break;
	    case Electromagnet4:
	      gpio_set(PTA13, 1);
	      break;
       }
}


void electromagnet_off(uint8 Electromagnet_Number)   //关
{
       switch(Electromagnet_Number)
       {
	    case Electromagnet1:
	      gpio_set (PTA10, 0);
	      break;
	    case Electromagnet2:
	      gpio_set (PTA11, 0);
	      break;
	    case Electromagnet3:
	      gpio_set (PTA12, 0);
	      break;
	    case Electromagnet4:
	      gpio_set (PTA13, 0);
	      break;
       }
}

