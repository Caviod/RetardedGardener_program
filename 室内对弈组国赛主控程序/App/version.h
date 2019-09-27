#ifndef __VERSION_H__
#define  __VERSION_H__



#define Qopenmv_UART_Port UART0
#define Yopenmv_UART_Port UART2


#define Qopenmv_Vector  UART0_RX_TX_VECTORn
#define Yopenmv_Vector  UART2_RX_TX_VECTORn

#define Qopenmv_IRQn  UART0_RX_TX_IRQn
#define Yopenmv_IRQn  UART2_RX_TX_IRQn


#define Openmv_Baud 115200


typedef struct
{
       int Mid_X;
       int Mid_Y;//接收到的方格的中心所在行值
       int Curl;    //旋度

       unsigned char RxBuffer[5];
       unsigned char RxCnt;
       unsigned char Block_Count_Flag;
       /*数格子相关变量*/
       int Block_Count;//格子计数
       int Mid_Y_Last;//上一次接收的方格中心所在行值
       int Mid_X_Last;
       int Block_Jump_flag;//判断方格跳变的标志位
       int Jump_Count;//跳变后的维持计数
}Str_Openmv;


extern void version_init();
extern Str_Openmv  Qopenmv;
extern Str_Openmv  Yopenmv;

#endif