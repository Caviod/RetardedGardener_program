/******************************(C) COPYRIGHT 2019 SMU智能车****************************
*     @file           CAN_Control.c
*     @brief          CAN总线控制
*     @note
*     @history
*  Version    Date            Author          Modification
*  V1.0.1     2019.5.19       min               finish
****************************(C) COPYRIGHT 2019 SMU智能车*****************************/
#include "include.h"
#include "CAN_Control.h"

//*************发送总线变量***************
uint8 CAN_Control_Data[8];              /*CAN控制数组*/
CAN_USR_ID_t can_tx_id = {CAN_TX_ID,0,0};/*CAN的ID数组*/
str_MotorData  MotorData[4];
//*************接收总线变量***************
uint8 CAN_Receive_Data[8];              /*CAN接收数组*/
uint8 rx_buf_len;                       /*CAN接收数据长度*/
uint8 Motor_Data[4][8];                 /*底盘四个2006电机反馈的实时数据*/
uint8 CAN_Receive_Flag;                 /*CAN接收到数据标志*/
CAN_USR_ID_t can_rx_id;                 /*CAN接收ID*/
uint16   CAN_Receive_id ;
//************定义函数*********************
void CAN_Get_Message();
void can0_mb_handler();//接收中断
void Chassis_CAN_Cmd(int16 Motor1_Speed,int16 Motor2_Speed,int16 Motor3_Speed,int16 Motor4_Speed);//底盘CAN总线命令函数


/*******************************************分割线***********************************************/
/*
* @brief          CAN初始化函数
* @author             闵启玄
* @retval             返回空
*/
void CAN_Init()           //底盘CAN总线初始化
{
       can_init(Chassis_CAN_CH,CAN_BAUD_1M,CAN_NORMAL,CAN_CLKSRC_BUS);                      //初始化can，波特率1MHZ，正常模式 ,bus 时钟作为时钟源
       set_vector_handler(CAN_VECTOR,can0_mb_handler);                                       //配置CAN接收中断服务函数到中断向量表
       can_rxbuff_enble(Chassis_CAN_CH,CAN_RX_MB,can_rx_id);                                  //使能接收缓冲区
       can_irq_en(Chassis_CAN_CH,CAN_RX_MB);                                                 //使能CAN接收中断
}

/**
* @brief          接收CAN总线发送过来的数据
* @param[in]      Motor_Speed:四个电机的总线给值 - 可正可负
* @author         闵启玄
* @retval         返回空
*/
void Chassis_CAN_Cmd(int16 Motor1_Speed,int16 Motor2_Speed,int16 Motor3_Speed,int16 Motor4_Speed)
{
       can_irq_dis(Chassis_CAN_CH,CAN_RX_MB);
       CAN_Control_Data[0]=(Motor1_Speed>>8)&0xff;               //控制电流的高八位
       CAN_Control_Data[1]=Motor1_Speed&0xff;                    //控制电流的低八位

       CAN_Control_Data[2]=(Motor2_Speed>>8)&0xff;               //控制电流的高八位
       CAN_Control_Data[3]=Motor2_Speed&0xff;                    //控制电流的低八位

       CAN_Control_Data[4]=(Motor3_Speed>>8)&0xff;               //控制电流的高八位
       CAN_Control_Data[5]=Motor3_Speed&0xff;                    //控制电流的低八位

       CAN_Control_Data[6]=(Motor4_Speed>>8)&0xff;               //控制电流的高八位
       CAN_Control_Data[7]=Motor4_Speed&0xff;                    //控制电流的低八位

       can_tx(Chassis_CAN_CH,CAN_TX_MB,can_tx_id,8,CAN_Control_Data);
       can_irq_en(Chassis_CAN_CH,CAN_RX_MB);
}

/**
* @brief          接收CAN总线发送过来的数据
* @author         闵启玄
* @retval         返回空
*/
void CAN_Get_Message()//CAN接收数据，该函数由中断触发
{
       can_rx(Chassis_CAN_CH,CAN_RX_MB,&can_rx_id,&rx_buf_len,CAN_Receive_Data);//接收数据
       switch (can_rx_id.ID)
       {
	    case 0x201:
	      {
		     MotorData[0].Rpm =CAN_Receive_Data[2]<<8|(CAN_Receive_Data[3]&0xff);
		     break;
	      }
	    case 0x202:
	      {
		     MotorData[1].Rpm  =CAN_Receive_Data[2]<<8|(CAN_Receive_Data[3]&0xff);
		     break;
	      }
	    case 0x203:
	      {
		     MotorData[2].Rpm =CAN_Receive_Data[2]<<8|(CAN_Receive_Data[3]&0xff);
		     break;
	      }
	    case 0x204:
	      {
		     MotorData[3].Rpm =CAN_Receive_Data[2]<<8|(CAN_Receive_Data[3]&0xff);
		     break;
	      }
       }
       gpio_turn(PTE26);   //反转电平
}

/**
* @brief          CAN总线接收中断服务函数
* @author
* @retval         返回空
*/
void can0_mb_handler(void)
{
       can_clear_flag(Chassis_CAN_CH,CAN_RX_MB);        //清除缓冲区中断标志位
       CAN_Get_Message();
}

