/**
  ****************************(C) COPYRIGHT 2019 SMU智能车实验室****************************
  *     @file           remote.c
  *     @brief          遥控库
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2019.6.10      陈云帆                Finish
  ****************************(C) COPYRIGHT 2019 SMU智能车实验室****************************
  */
#include "remote.h"
#include "include.h"


/*************************************************************/
/*****************遥控部分（模拟量）************/
/*************************************************************/
//定义遥控器初始位置
void Chassist_init(ReceiveCMDData *Initial_Remote)
{
       Initial_Remote->Right_X=127;
       Initial_Remote->Right_Y=127;
       Initial_Remote->Left_X=127;
       Initial_Remote->Left_Y=127;
}
/// <summary>
///初始化遥控器的串口
///</summary>
void RemoteInit()
{
       uart_init(Remote_Uart_Port,9600);
       set_vector_handler(UART1_RX_TX_VECTORn,ReceiveCMD_Remote);           //UART1
       uart_rx_irq_dis(Remote_Uart_Port);
       uart_rx_irq_en(Remote_Uart_Port);
}

int ReceiveIndex = 0;
unsigned char ReceiveBuff[3] = {0};
unsigned char StartReceive = 0;
RemoteCMDMode RunMode;//遥控模式
//#ifdef Remote_UseDigitalReceive

Remote_State Remote_CMD_ReceiveStatus = Sleep;
ReceiveCMDData RemoteData;
long count_error_left = 0;
long count_error_right = 0;
unsigned char Flag_RemoteStopCar = 0;
unsigned char Flag_StartPressOnce = 0;


void GetRemoteCMDData(void)
{
       if(Flag_RemoteStopCar != 0)
	      return;
       if (Remote_CMD_ReceiveStatus==Sleep)
       {
	      Remote_CMD_ReceiveStatus = SendLeftCMD;
	      Remote_CMD_ReceiveStatus = ReceivingLeftCMD;
	      uart_putchar(Remote_Uart_Port, 0xBB);
       }
       if(Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
       {
	      count_error_left++;
	      count_error_right = 0;
       }
       else if(Remote_CMD_ReceiveStatus == ReceivingRightCMD)
       {
	      count_error_left = 0;
	      count_error_right++;
       }
       else
       {
	      count_error_left = 0;
	      count_error_right = 0;
       }
       if(count_error_left>=180||count_error_right>=180)
       {
	      RemoteData.Left_Y  = 127;
	      RemoteData.Left_X  = 127;
	      RemoteData.Right_Y = 127;
	      RemoteData.Right_X = 127;
	      Remote_CMD_ReceiveStatus  = Sleep;
	      count_error_left  = 0;
	      count_error_right = 0;
       }
}



//接受遥控指令程序，应放入对应的串口中断内
void ReceiveCMD_Remote()
{
       //用户需要处理接收数据
       unsigned char buff = 0;
       uart_getchar (Remote_Uart_Port,&buff);
       if (buff == 0xFF && StartReceive == 0)
       {
	      StartReceive = 1;
	      return;
       }
       else
       {
	      if (StartReceive == 0)
		     ReceiveIndex = 0;
       }
       if (StartReceive == 1)
       {
	      if (ReceiveIndex < 3)
	      {
		     ReceiveBuff[ReceiveIndex] = buff;
		     ReceiveIndex++;
	      }
	      else
	      {
		     if (buff == 0xFF)
		     {
			    StartReceive = 0;
			    ReceiveIndex = 0;
			    if (ReceiveBuff[0] == 0xBB)//左摇杆
			    {
				   if (Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
				   {
					  RemoteData.Left_Y = ReceiveBuff[1];
					  RemoteData.Left_X = ReceiveBuff[2];
					  Remote_CMD_ReceiveStatus=ReceivedLeftCMD;
					  Remote_CMD_ReceiveStatus=ReceivingRightCMD;
					  uart_putchar (Remote_Uart_Port, 0xCC);
				   }

			    }
			    else if (ReceiveBuff[0] == 0xCC)//右摇杆
			    {
				   if (Remote_CMD_ReceiveStatus == ReceivingRightCMD)
				   {
					  RemoteData.Right_Y = ReceiveBuff[1];
					  RemoteData.Right_X = ReceiveBuff[2];
					  Remote_CMD_ReceiveStatus = ReceivedRightCMD;
					  Remote_CMD_ReceiveStatus = Sleep;
				   }
			    }
			    else
			    {
				   RemoteData.Left_Y = 127;
				   RemoteData.Left_X = 127;
				   RemoteData.Right_Y = 127;
				   RemoteData.Right_X = 127;
				   Remote_CMD_ReceiveStatus = Sleep;
			    }
			    if(ReceiveIndex == 0 && ReceiveBuff[0] == 0xAA)
			    {
				   if(Flag_RemoteStopCar == 0)
					Flag_RemoteStopCar = 1;
				   else if (Flag_RemoteStopCar >= 1)
					Flag_RemoteStopCar++;
				   if(Flag_RemoteStopCar >= 4)
					Flag_RemoteStopCar = 0;
                                     if(Flag_StartPressOnce == 0)
                                          Flag_StartPressOnce = 1;
			    }

		     }
	      }
       }

}



#ifndef Remote_UseDigitalReceive
///<summary>将从遥控器上获得数值转化为整车运动速度（x、y轴平动速度和自转速度）</summary>
void SetSpeed_FromRemote_Analog(void)
{
  get_XYZ((RemoteData.Left_X-127)*14000/127, (RemoteData.Left_Y-127)*14000/127, (127-RemoteData.Right_X)*90000/127);
}
#endif

void ReceiveCollect();
/// <summary>
///初始化前摄像头采集板的串口
///</summary>
void CollectInit()
{
  uart_init(Collect_Uart_Port, 115200);
  set_vector_handler(UART5_RX_TX_VECTORn,ReceiveCollect);
  uart_rx_irq_dis(Collect_Uart_Port);                                   //关闭接收中断
  uart_rx_irq_en(Collect_Uart_Port);
}
int ReceiveCollectIndex = 0;
unsigned char ReceiveCollectBuff[4] = {0};
unsigned char StartCollectReceive = 0;
uint8 HLineLocation = 0;
uint8 HLineLocation_Right = 0;
//前摄像头采集板串口接受程序
void ReceiveCollect()
{
      unsigned char buff = 0;
      uart_getchar (Collect_Uart_Port,&buff);
      if (buff == 0xFF && StartCollectReceive == 0)
      {
        StartCollectReceive = 1;
        ReceiveCollectBuff[ReceiveCollectIndex] = buff;
        ReceiveCollectIndex++;
        return;
      }
      else
      {
        if (StartCollectReceive == 0)
          ReceiveCollectIndex = 0;
      }
      if (StartCollectReceive == 1)
      {
        if (ReceiveCollectIndex < 3)
        {
          ReceiveCollectBuff[ReceiveCollectIndex] = buff;
          ReceiveCollectIndex++;
        }
        else
        {
          if(buff == 0xFE)
          {
            ReceiveCollectBuff[ReceiveCollectIndex] = buff;
            HLineLocation = ReceiveCollectBuff[1];
            HLineLocation_Right  = ReceiveCollectBuff[2];
            CountGrid();
          }
          StartCollectReceive = 0;
          ReceiveCollectIndex = 0;
        }
      }
}



