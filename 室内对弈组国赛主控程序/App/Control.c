#include "Control.h"
/*===================================================================
功能：参数控制
===================================================================*/


//直立环
float ANGLE_Set = 53;
float P_ANGLE = 200;
float D_ANGLE = 75;
//速度环
int16 Speed_Set = 0;
float P_SPEED = 0;
float I_SPEED = 0;
//方向环
float DIRC_P = 0;
float DIRC_D = 0;

/*===================================================================
功能：卡尔曼滤波
===================================================================*/
float  Angle_dot=0;
float  Angle=0;
float  Gyro_x=0;
float  Q_angle=0.0001;//0.0001;
float  Q_gyro=0.00003;//0.00003;
float  R_angle=0.03;
float  dt=0.005;//0.005;
float  C_0 = 1.0;
float  Q_bias=0, Angle_err=0;
float  PCt_0=0.0, PCt_1=0, E=0.0;
float  K_0=0.0, K_1=0.0, t_0=0.0, t_1=0.0;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1.0, 0.0 },{ 0.0, 1.0 } };
void Kalman_Cal(float Gyro,float Accel)
{
  Angle+=(Gyro - Q_bias) * dt;
  Angle_err =Accel - Angle;

  Pdot[0]=Q_angle - PP[0][1] - PP[1][0];

  Pdot[1]=- PP[1][1];
  Pdot[2]=- PP[1][1];
  Pdot[3]=Q_gyro;

  PP[0][0] += Pdot[0] * dt;
  PP[0][1] += Pdot[1] * dt;
  PP[1][0] += Pdot[2] * dt;
  PP[1][1] += Pdot[3] * dt;

  PCt_0 = C_0 * PP[0][0];
  PCt_1 = C_0 * PP[1][0];

  E = R_angle + C_0 * PCt_0;

  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;

  t_0 = PCt_0;
  t_1 = C_0 * PP[0][1];

  PP[0][0] -= K_0 * t_0;
  PP[0][1] -= K_0 * t_1;
  PP[1][0] -= K_1 * t_0;
  PP[1][1] -= K_1 * t_1;

  Angle    += K_0 * Angle_err;
  Q_bias   += K_1 * Angle_err;
  Angle_dot = Gyro - Q_bias;
}
/*===================================================================
功能:：一阶互补滤波
备注:：deltaT采样周期 ，tau时间常数，建议值1.2f
====================================================================*/
/*-----------------------互补滤波器系数计算------------------------*/
float CF_Factor_Cal(float deltaT, float tau)
{
  return tau / (deltaT + tau);
}
/*------------------------一阶互补滤波器---------------------------*/
float CF_1st(float gyroData, float accData, float cf_factor)
{
  return (gyroData * cf_factor + accData *(1 - cf_factor));
}
/*---------------------改进型一阶互补滤波器------------------------*/
float CF_1st_EX(float oldData,float gyroData, float accData, float cf_factor,float deltaT)
{
  return ((gyroData*deltaT+oldData )*cf_factor + accData *(1 - cf_factor));
}
/*---------------------------主线程--------------------------------*/
const float DeltaT = 1.2;
const float Tau = 1.2;
float Databuff = 0;
//float Complementary_Filtering(float AX,float GY)
//{
//  Databuff = CF_1st_EX(Databuff,GY,AX, CF_Factor_Cal(DeltaT, Tau),DeltaT);
//  return Databuff;
//}
/*===================================================================
功能:：二阶互补滤波
====================================================================*/
float K2 =0.2; // 对加速度计取值的权重
float x1,x2,y1;//运算中间变量
float angle2;
void DComplementary(float angle_m, float gyro_m)
{
    x1=(angle_m-angle2)*(1-K2)*(1-K2);
    y1=y1+x1*dt;
    x2=y1+2*(1-K2)*(angle_m-angle2)+gyro_m;
    angle2=angle2+ x2*0.02;
    Angle = angle2;
}

/*===================================================================
功能：编码器测速
===================================================================*/
//int16 temp_Left = 0, temp_Right = 0;
int16 Speed;
//int16 SpeedGet(void)
//{
//  int temp_L = FTM_QUAD_get(FTM1);
//  int temp_R = FTM_QUAD_get(FTM2);
//  if (gpio_get(PTA17) == 0)temp_L = -temp_L;
//  FTM_QUAD_clean(FTM1);
//  temp_Left += temp_L;
//  if (gpio_get(PTA9) == 1)temp_R = -temp_R;
//  FTM_QUAD_clean(FTM2);
//  temp_Right += temp_R;
//  Speed = (temp_Left + temp_Right) / 2;
//  return Speed;
//}
/*===================================================================
功能：直立控制
控制器：位置式PD控制器
===================================================================*/
float nAngleControlOut = 0;
float speed_Start;
void AngleControl(void)
{
  //AX = (GetAccelX() - ACC_OFFSET_X) * 0.0001220703125;
  //GY = (GetAnguY() - GYRO_OFFSET_Y) * 0.0152671755725;
  //AX = (GetAccelX() - ACC_OFFSET_X);
  //GY = (GetAnguY() - GYRO_OFFSET_Y);
  //float Angle_Error;
 // static float Angle_Error_Buff = 0;
  //ANGLE = (Kalman_Cal(AX,GY,0.005)+0.394)*10.965-2;
  //Kalman_Cal(GY, AX);
  //Angle = Complementary_Filtering(AX,GY);
//  Angle_Error = Angle - ANGLE_Set;
//  speed_Start = P_ANGLE * Angle_Error + D_ANGLE * Angle_dot;
//  if (speed_Start > MOTOR_MAX_Z) speed_Start = MOTOR_MAX_Z;
//  if (speed_Start < MOTOR_MIN_F) speed_Start = MOTOR_MIN_F;
//  Angle_Error_Buff = Angle_Error;
//  nAngleControlOut = speed_Start;
}
/*===================================================================
功能：速度控制
控制器：增量式PI控制器
===================================================================*/
float nSpeedControlOut = 0;
void SpeedControl(void)
{
  static int16 Speed_Error_Buff = 0;
  int16 Speed_Error = Speed - Speed_Set;
  nSpeedControlOut = P_SPEED * (Speed_Error - Speed_Error_Buff) + I_SPEED * Speed_Error;
  Speed_Error_Buff = Speed_Error;
}
/*===================================================================
功能：方向控制
控制器：离散化位置式PD控制器
===================================================================*/
float nDirControlOut = 0;
int piancha;
float nDirControlOutNew = 0, nDirControlOutOld = 0;
int8 nDirectionControlPeriod = 0;
extern float GYRO_OFFSET_X;
void DirectionControl(void)
{
  float GX = (GetAnguX() - GYRO_OFFSET_X) / 65.5;
  nDirControlOutOld = nDirControlOutNew;
  nDirControlOutNew = piancha * DIRC_P - GX * DIRC_D;
}
void DirectionControlOutput(void)
{
  float Value = nDirControlOutNew - nDirControlOutOld;
  nDirControlOut = Value * (nDirectionControlPeriod + 1) / 2 + nDirControlOutOld;
}
/*===================================================================
功能：电机控制
===================================================================*/
float nLeftVal, nRightVal;
void MotorOutput(void)
{
  nLeftVal = nAngleControlOut - nSpeedControlOut + nDirControlOut;
  nRightVal = nAngleControlOut - nSpeedControlOut - nDirControlOut;
  if (nLeftVal > 0)
  {
    nLeftVal += MOTOR_DEAD_VAL_L;
    if (nLeftVal > MOTOR_MAX_Z) nLeftVal = MOTOR_MAX_Z;
    if (nLeftVal < MOTOR_MIN_Z) nLeftVal = MOTOR_MIN_Z;
  }
  else if (nLeftVal <= 0)
  {
    nLeftVal -= MOTOR_DEAD_VAL_L;
    if (nLeftVal < MOTOR_MIN_F) nLeftVal = MOTOR_MIN_F;
    if (nLeftVal > MOTOR_MAX_F) nLeftVal = MOTOR_MAX_F;
  }
  if (nRightVal > 0)
  {
    nRightVal += MOTOR_DEAD_VAL_R;
    if (nRightVal > MOTOR_MAX_Z) nRightVal = MOTOR_MAX_Z;
    if (nRightVal < MOTOR_MIN_Z) nRightVal = MOTOR_MIN_Z;
  }
  else if (nRightVal <= 0)
  {
    nRightVal -= MOTOR_DEAD_VAL_R;
    if (nRightVal < MOTOR_MIN_F) nRightVal = MOTOR_MIN_F;
    if (nRightVal > MOTOR_MAX_F) nRightVal = MOTOR_MAX_F;
  }
  SetMotorVoltage();
}
//void SetMotorVoltage(void)
//{
//  if (nLeftVal >= 0)
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH3, 0);
//    FTM_PWM_Duty(FTM0, FTM_CH4, nLeftVal);
//  }
//  else
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH4, 0);
//    FTM_PWM_Duty(FTM0, FTM_CH3, -nLeftVal);
//  }
//  if (nRightVal >= 0)
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH1, 0);
//    FTM_PWM_Duty(FTM0, FTM_CH2, nRightVal);
//  }
//  else
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH2, 0);
//    FTM_PWM_Duty(FTM0, FTM_CH1, -nRightVal);
//  }
//}






