/**
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           CAN_Control.h
  *     @brief          CAN总线控制头文件
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V1.0.1     2019.5.19       min               finish
  ****************************(C) COPYRIGHT 2019 SMU智能车****************************
  */
#ifndef __CAN_CONTROL_H
#define __CAN_CONTROL_H


#define CAN_TX_ID          0x200
#define CAN_RX_ID          0x201


#define Chassis_CAN_CH    CAN0
#define CAN_VECTOR        CAN0_ORed_MB_VECTORn

#define CAN_TX_MB       MB_NUM_7            //发送MBs索引定义
#define CAN_RX_MB       MB_NUM_6            //接收MBs索引定义
#define DATA_LEN        8                   //can 发送的数据长度，最大为8




typedef struct
{
  short Angle;  //机械角度
  short Rpm;    //转速
  short Torque ;// 转矩
  short Speed_Out ;//
}str_MotorData;

extern str_MotorData  MotorData[4];

extern void CAN_Init();
extern void Chassis_CAN_Cmd(int16 Motor1_Speed,int16 Motor2_Speed,int16 Motor3_Speed,int16 Motor4_Speed);

extern uint16 CAN_Receive_id ;                 /*CAN接收ID*/
extern uint8 rx_buf_len;                       /*CAN接收数据长度*/
extern uint8 CAN_Receive_Buf[16];              /*CAN接收数组*/
extern uint8 CAN_Receive_Flag;                  /*CAN接收到数据标志*/
extern uint8 Motor_Data[4][8];                  /*底盘四个电机的CAN总线反馈数据*/

#endif