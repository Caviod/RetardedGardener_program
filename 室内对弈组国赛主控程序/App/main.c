#include "common.h"
#include "include.h"


//初始化硬件
void BSP_Init(void)
{
       DisableInterrupts;
       /********************中断优先级设置*******************/
       NVIC_SetPriorityGrouping(4);
       NVIC_SetPriority(CAN0_ORed_MB_IRQn, 0);//can总线中断优先级最高
       NVIC_SetPriority(Qopenmv_IRQn, 1);   //OPENMV接受
       NVIC_SetPriority(Yopenmv_IRQn, 1);   //OPENMV接受
       NVIC_SetPriority(PIT0_IRQn,2);     //见misc.h    set_irq_priority(PORTD_IRQn,3);  此函数在core_cm4.h中
       NVIC_SetPriority(PIT1_IRQn, 3);
       /*****************DMA通道优先级设置*****************/
       led_init (LED_MAX);
       key_init (KEY_MAX);                                        //按键初始化
       /**********************************************************************************/
gpio_init(PTC13,GPO, 1);
/**********************************************************************************/
       pushrod_init();                                               //推杆初始化
/**********************************************************************************/
       electromagnet_init();                                         //电磁铁初始化
/**********************************************************************************/
       RemoteInit();                                                 //遥控器初始
/**********************************************************************************/
/**********************************************************************************/
       //version_init();                                               //视觉初始化
/**********************************************************************************/
       //GY901_init();
       PlannerError_uart_init();                                             //陀螺仪初始化
       InitMPU6050();
/**********************************************************************************/
       LCD_Init();                                                   //OLED初始化
//       pit_time_start(PIT1);                                         //航姿推测计时
#if 1
       pit_time_start(PIT2);
       PIT_INIT();
#endif
       
/**********************************************************************************/
       Chassist_init(&RemoteData);        //PAODONG
       chassis_init();                                               //底盘初始化
       EnableInterrupts;
}

unsigned char Flag_Holding = 0;
unsigned char Flag_Downing = 0;
extern unsigned char TaskIndex;


void OLED_Display()
{
    
}
unsigned long TimeMain = 0;
unsigned char Flag_SendIMUData = 0;
void  main(void)

{
       BSP_Init();  //初始化
//       Dead_Recking_Reset(0, 0, 0);
       while(1)//1.9ms
       {
         Achieve_Gyro();
         //              Dead_Recking();
         if (Flag_Holding)
         {
           if (Check_HoldOrDownFinish())
           {
             Flag_Holding = 0;
           }
         }
         if (Flag_Downing)
         {
           if (Check_HoldOrDownFinish())
           {
             Flag_Downing = 0;
           }
         }
         
         if(Flag_SendIMUData) 
         {
           Flag_SendIMUData=0;
           Gyro_Send();//1.7ms
         }
         OLED_PrintFloat(0,0,DRx);
         OLED_PrintFloat(0,1,DRy);
         OLED_PrintFloat(0,2,DRw);
         LCD_Print16(0,3,gz);
         OLED_PrintFloat(0,4,Distance);
       }
}

