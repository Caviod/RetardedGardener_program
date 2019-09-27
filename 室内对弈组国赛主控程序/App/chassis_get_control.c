#include "chassis_get_control.h"
#include "include.h"

//uart初始化 发送MPU6050数据 接收朝向角偏差 xy距离偏差 储存在PlannerError_Origin结构体中
void PlannerError_uart_init()
{
  uart_init(UART5,115200);
  set_vector_handler(UART5_RX_TX_VECTORn,PlannerError_receive);
  uart_rx_irq_dis(UART5);
  uart_rx_irq_en(UART5);
}
//采集陀螺仪数据--放入定时器中定时采集
struct IMU_2D imu_2d_raw;
void Achieve_Gyro()
{
  Get_IMU2D(&imu_2d_raw, &Accel_OriginData, &GYRO_OriginData);
}

#define FILL_INT16(arr, x) {\
  (arr)[0] =  (int)(x) & 0xff;\
  (arr)[1] = ((int)(x)>>8) & 0xff;\
}
#define FILL_FLOAT(arr, x) {\
  *(float *)(arr) = x;\
}
unsigned char SendRunModeCount = 0;
uint8 Send[23];
void Create_Send()
{
  int wheel[4] = {
    MotorData[0].Rpm,
    MotorData[1].Rpm,
    MotorData[2].Rpm,
    MotorData[3].Rpm
  };
  int vx, vy, wz;
  chassis_inverse(&vx, &vy, &wz, wheel);

  Send[0]=0x55;
  Send[1]=0xAA;

  FILL_INT16(&Send[2], imu_2d_raw.GZ);
  FILL_INT16(&Send[4], vx);
  FILL_INT16(&Send[6], vy);

  FILL_FLOAT(&Send[8], DRw);
  FILL_FLOAT(&Send[12], DRx);
  FILL_FLOAT(&Send[16], DRy);

  Send[20] = RunPolicyMode;//0 代表运行中， 1代表进行下一步决策， 2代表重新进行上一次决策
  uint16 mix = imu_2d_raw.GZ + vx + vy + (int)(DRx + DRy + DRw) + RunPolicyMode;

  FILL_INT16(&Send[21], mix);
  
  if(RunPolicyMode != Doing)
  {    
    if(SendRunModeCount >= 20)
    {
      RunPolicyMode = Doing;
      SendRunModeCount = 0;
    }  
    else
      SendRunModeCount++;
  }
}
//向上位机发送陀螺仪数据
void Gyro_Send()
{
  Create_Send();
  for(int i=0;i<sizeof(Send);i++)
   uart_putchar(UART5,Send[i]);
}


PlannerError PlannerError_Origin;
//uart接收朝向角与距离
int planner_error_receive_index=0;
int planner_error_start_receive=0;
uint8 get_planner_error[15];

const unsigned char preamble[] = {0x55, 0xAA};
int preamble_pos = 0;
unsigned char chksum = 0;
unsigned char message[50];
int message_size = -1;
int message_pos = 0;

void do_electromagnet(int enable) {
  if(enable == 0)
  {
    electromagnet_off(0);
    electromagnet_off(1);
    electromagnet_off(2);
    electromagnet_off(3);
    
  }
  else
  {
    electromagnet_on(0);
    electromagnet_on(1);
    electromagnet_on(2);
    electromagnet_on(3);
  }
}

void do_message(void *message, int size) {
  int type = ((unsigned char *)message)[0];
  void *body = (unsigned char *)message + 1;
  switch (type) {
  case 0: // 重置命令 float x3
    Dead_Recking_Reset(
                       ((float *)(body))[1],
                       ((float *)(body))[2],
                       ((float *)(body))[0]);
    break;
  case 1: // 目的地命令 float x3
    PlannerError_Origin.target_angle = ((float *)(body))[0];
    PlannerError_Origin.target_x = ((float *)(body))[1];
    PlannerError_Origin.target_y = ((float *)(body))[2];
    break;
  case 2: // 修正坐标命令 float x3
    CORRw = ((float *)(body))[0];
    CORRx = ((float *)(body))[1];
    CORRy = ((float *)(body))[2];
    break;
  case 3: // 推杆命令  uint8 x1
    DoTaskEx(((unsigned char *)body)[0]);
    break;
  case 4: // 电磁铁命令  uint8 x1
    do_electromagnet(((unsigned char *)body)[0]);
    break;
  case 5: // 设置最高速度 int32 x1
    speed_max = *(int32 *)body;
    break;
  case 6:
    
    
  }
}

void PlannerError_receive()
{
  unsigned char buff=0;
  uart_getchar(UART5,&buff);
  /*

[55 AA] Size [XX XX XX XX XX] Chksum

*/
  if (preamble_pos < sizeof(preamble)) {
    if (buff == preamble[preamble_pos]) {
      preamble_pos++;
    } else {
      preamble_pos = 0;
      return;
    }
    if (preamble_pos == sizeof(preamble)) {
      message_size = -1;
      chksum = 0;
    }
    return;
  }
  if (message_size == -1) {
    message_size = buff;
    message_pos = 0;
    return;
  }
  if (message_pos == message_size) {
    preamble_pos = 0;
    if (buff == chksum) do_message(message, message_size);
    return;
  }
  message[message_pos] = buff;
  chksum += buff;
  message_pos++;
  return;
}

/***********************************************************
航姿推测

* 函数名称： Dead_Recking(void)
* 功能说明： 航姿推测计算
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
************************************************************/
long Delta_Time=0;
long Time_Now=0,Time_Last=0;
float DRx = 0, DRy = 0, DRw = 0;
float CORRx=0,CORRy=0,CORRw=0;
int16 gz = 0;
float Distance = 0;
void Dead_Recking()
{

  //开始计时
//  Time_Now=pit_time_get_ms(PIT1);
  int wheel[4] = 
  {
    MotorData[0].Rpm,
    MotorData[1].Rpm,
    MotorData[2].Rpm,
    MotorData[3].Rpm
  };
  int vx,vy,wz;
  chassis_inverse(&vx,&vy,&wz,wheel);
  gz = (int16)imu_2d_raw.GZ;
  //公制化单位
  float Gz,Vx,Vy;
  Gz=gz*(500.0/32768/180*PI);
  Vx=vx/4321.35525 / 1.05540897 / (4/(4-0.06));
  Vy=vy/4381.02651 / 1.00175307/1.02564 / (4/(4-0.09));
  
  Delta_Time=10;//Time_Now-Time_Last;
//  if(Delta_Time<0)
//  {
//    Time_Last=0;
//    return;
//  }
  
  
  Distance=sqrt(Vx*Vx+Vy*Vy)*Delta_Time/1000.0;
  float theta=atan2(Vy,Vx)+DRw+CORRw;
 
  DRx += Distance * cos(theta);
  DRy += Distance * sin(theta);
  DRw += Gz * Delta_Time/1000.0;
  
//  LCD_PrintU16(0,5,(uint16)Time_Now);
  
//  Time_Last=Time_Now;
//  if(Time_Last==0 && Time_Now==0)
//  {
//    pit_close(PIT1);
//    pit_time_start(PIT1);
//  }
  
}

//重置航姿推测的积分距离
void Dead_Recking_Reset(float x, float y, float angle)
{
  DRx = x;
  DRy = y;
  DRw = angle;
  CORRx=0;
  CORRy=0;
  CORRw=0;
  PlannerError_Origin.target_angle = angle;
  PlannerError_Origin.target_x = x;
  PlannerError_Origin.target_y = y;
  PlannerError_Origin.next_action = 0;
}
