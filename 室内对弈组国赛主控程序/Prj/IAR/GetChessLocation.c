#include "GetChessLocation.h"

void GetChessLocation_Init();
void GetChess_Location();
//任务列表接收初始化
void GetChessLocation_Init()
{
  uart_init(GetLocaction_Uart_Port,9600);
  set_vector_handler(UART0_RX_TX_VECTORn,GetChess_Location);
  uart_rx_irq_dis(GetLocaction_Uart_Port);                                   
  uart_rx_irq_en(GetLocaction_Uart_Port);
}

//任务列表接收程序 包头FF 包尾FE
//Forward 0 Back 1 Left 2 Right 3 Stop 4 HoldChess 5 Downchess 6 +多少步
unsigned char Task_Chart[100]; //任务表
unsigned char Start_Receive=0;
unsigned char Receive_Index=0;
void GetChess_Location()
{
  unsigned char buff=0;
  uart_getchar(GetLocaction_Uart_Port,&buff)
  if (buff == 0xFF && Start_Receive == 0)
  {
    Start_Receive=1;
     return;
  }
  else
  {
    if(Start_Receive==0)
       Receive_Index=0;
  }
  if(Start_Receive==1)
  {
    if(buff==0xFE)
    {
      Start_Receive=0;
      Receive_Index=0;
    }
    else
    {
      Task_Chart[Receive_Index]=buff;
      ConfigTask(&EQ_TaskPool[Receive_Index],Task_Chart[Receive_Index]/10,Task_Chart[Receive_Index]%10);
      Receive_Index++;
    }
  }
}

