#include "gyrostat.h"
#include "include.h"


 /*******************************************
  陀螺仪结构体声明
*******************************************/
struct STime  stcTime;
struct SAcc   stcAcc;
struct SGyro  stcGyro;
struct SAngle stcAngle;
struct SMag   stcMag;
struct SDStatus stcDStatus;
struct SPress  stcPress;
struct SLonLat stcLonLat;
struct SGPSV  stcGPSV;
struct SQ  stcQ;

void GY901_handler(void);


/*******************************************
陀螺仪初始化
*******************************************/
void GY901_init()
{
       uart_init(UART5,115200);
       set_vector_handler(UART5_RX_TX_VECTORn, GY901_handler); //注释我
       //enable_irq(UART3_RX_TX_IRQn);                         //注释我
       uart_rx_irq_en(UART5);
}

//输出时间
//printf("Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
//加速度
//printf("Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
//角速度
//printf("Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
//角度
//printf("Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
//磁场
//printf("Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);

float X[3]={0,0,0},Y[3]={0,0,0},Z[3]={0,0,0};
short Wx,Wy,Wz;


double pitch;	// pitch
double roll;// roll
double yaw;

double q0;
double q1;
double q2;
double q3;

void gyrostst(void)
{
#if 0
       X[1]=X[0];
       Y[1]=Y[0];
       Z[1]=Z[0];

       X[0]=stcAngle.Angle[0]*0.005493164;
       Y[0]=stcAngle.Angle[1]*0.005493164+4;
       Z[0]=stcAngle.Angle[2]*0.005493164;

       Wx=stcGyro.w[0];
       Wy=stcGyro.w[1];
       Wz=stcGyro.w[2];
#endif
       pitch = asin(-2.0 * q1 * q3 + 2.0 * q0* q2)* 57.3;	// pitch
       roll  = atan2(2.0 * q2 * q3 + 2.0 * q0 * q1, -2.0 * q1 * q1 - 2.0 * q2* q2 + 1.0)* 57.3;	// roll
       yaw   = atan2(2.0*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw

////       LCD_P6x8Str(0,1,"Roll");
////
//      OLED_PrintFloat(60 , 1,roll);
////
////       LCD_P6x8Str(0,2,"Pitch");
////
//      OLED_PrintFloat(60 , 2, pitch);
////
////       LCD_P6x8Str(0,3,"Yaw");
////
//      OLED_PrintFloat(60 , 3,yaw);
}


/*******************************************
陀螺仪中断
*******************************************/
static uint8 ucRxBuffer[250];
static char ucRxCnt = 0;
void GY901_handler(void)
{
       uart_getchar(GY901_UART, &ucRxBuffer[ucRxCnt++]);
       gpio_turn(PTC0);   //反转电平
       if (ucRxBuffer[0] != 0x55) //判断数据头
       {
	      ucRxCnt = 0;
	      return;
       }
       if (ucRxCnt < 11)
       {
	      return;
       }
       else
       {
	      switch (ucRxBuffer[1])
	      {
		   case 0x50:
		     memcpy(&stcTime, &ucRxBuffer[2], 8);
		     break;
		   case 0x51:
		     memcpy(&stcAcc, &ucRxBuffer[2], 8);
		     break;
		   case 0x52:
		     memcpy(&stcGyro, &ucRxBuffer[2], 8);
		     break;
		   case 0x53:
		     memcpy(&stcAngle, &ucRxBuffer[2], 8);
		     break;
		   case 0x54:
		     memcpy(&stcMag, &ucRxBuffer[2], 8);
		     break;
		   case 0x55:
		     memcpy(&stcDStatus, &ucRxBuffer[2], 8);
		     break;
		   case 0x56:
		     memcpy(&stcPress, &ucRxBuffer[2], 8);
		     break;
		   case 0x57:
		     memcpy(&stcLonLat, &ucRxBuffer[2], 8);
		     break;
		   case 0x58:
		     memcpy(&stcGPSV, &ucRxBuffer[2], 8);
		     break;
		   case 0x59:
		     memcpy(&stcQ, &ucRxBuffer[2], 8);
		     break;
	      }
	      ucRxCnt = 0;

	      q0=stcQ.q[0]/32768.0;
	      q1=stcQ.q[1]/32768.0;
	      q2=stcQ.q[2]/32768.0;
	      q3=stcQ.q[3]/32768.0;
	     // pitch = asin(-2.0 * q1 * q3 + 2.0 * q0* q2)* 57.3;	// pitch
	      //roll  = atan2(2.0 * q2 * q3 + 2.0 * q0 * q1, -2.0 * q1 * q1 - 2.0 * q2* q2 + 1.0)* 57.3;	// roll
	      yaw   = atan2(2.0*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
       }

}
