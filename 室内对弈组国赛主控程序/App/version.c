#include "include.h"
#include "version.h"

Str_Openmv  Qopenmv={0};
Str_Openmv  Yopenmv={0};
/**********************************分割线*******************************************/
void X_block_count(Str_Openmv * OpenMV);

void Rec_Qopenmv_handler()
{
       uart_getchar(Qopenmv_UART_Port,&Qopenmv.RxBuffer[Qopenmv.RxCnt++]);
       if (Qopenmv.RxBuffer[0]!='A') //判断数据头
       {
	      Qopenmv.RxCnt = 0;
	      return;
       }
       if (Qopenmv.RxCnt < 4)
       {
	      return;
       }
       else
       {
	      Qopenmv.Mid_X=Qopenmv.RxBuffer[1]-80;
	      Qopenmv.Curl=Qopenmv.RxBuffer[2]-128;
	      Qopenmv.Mid_Y=Qopenmv.RxBuffer[3];
              X_block_count(&Qopenmv);
	      Qopenmv.RxCnt = 0;
       }
}

void Rec_Yopenmv_handler()
{
       uart_getchar(Yopenmv_UART_Port,&Yopenmv.RxBuffer[Yopenmv.RxCnt++]);
       if (Yopenmv.RxBuffer[0]!='A') //判断数据头
       {
	      Yopenmv.RxCnt = 0;
	      return;
       }
       if (Yopenmv.RxCnt < 4)
       {
	      return;
       }
       else
       {
	      Yopenmv.Mid_X=Yopenmv.RxBuffer[1]-80;
	      Yopenmv.Curl=Yopenmv.RxBuffer[2]-128;
	      Yopenmv.Mid_Y=Yopenmv.RxBuffer[3];
              X_block_count(&Yopenmv);
              
	      Yopenmv.RxCnt = 0;
       }
}

void Qopenmv_init()     //前OPENMV初始化
{
       uart_init(Qopenmv_UART_Port,Openmv_Baud);
       set_vector_handler(Qopenmv_Vector,Rec_Qopenmv_handler); //注释我
       uart_rx_irq_en(Qopenmv_UART_Port);
}




void Yopenmv_init()     //前OPENMV初始化
{
       uart_init(Yopenmv_UART_Port, Openmv_Baud);
       set_vector_handler(Yopenmv_Vector, Rec_Yopenmv_handler); //注释我
       uart_rx_irq_en(Yopenmv_UART_Port);
}



void version_init()
{
       Qopenmv_init();
       Yopenmv_init();
}

#define Jump_Standard 30//行跳变尺度
#define Jump_Keep 3//跳变后保持不调变的计数

void X_block_count(Str_Openmv * OpenMV)
{
  
  if(OpenMV->Block_Count_Flag==0&&OpenMV->Mid_X_Last!=0)
  {
    OpenMV->Block_Count_Flag=1;
    OpenMV->Block_Count++;
  }
  
    //如果跳变标志位没有被置位，说明没有发生跳变，如果这时候，前方格中心行值减去现中心行值大于30行，就判定发生了第一类跳变——前进跳变
  if (OpenMV->Mid_X_Last - OpenMV->Mid_X>Jump_Standard && OpenMV->Mid_X<0 &&OpenMV->Mid_X_Last>0)
    {
      OpenMV->Block_Count++;
    }
    //如果跳变标志位没有被置位，说明没有发生跳变，如果这时候，前方格中心行值减去现中心行值小于-30行，就判定发生了第二类跳变——后退跳变
    else if (OpenMV->Mid_X_Last - OpenMV->Mid_X<-Jump_Standard && OpenMV->Mid_X>0 &&OpenMV->Mid_X_Last<0)
    {
       OpenMV->Block_Count--;
    }
//SEND(10*Yopenmv.Block_Count, Yopenmv.Mid_X, Yopenmv.Mid_X_Last, 0);
    OpenMV->Mid_X_Last = OpenMV->Mid_X;
}

