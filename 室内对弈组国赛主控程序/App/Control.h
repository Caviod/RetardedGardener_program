#ifndef _IMU_H_
#define _IMU_H_

#include "include.h"

#define DIRECTION_CONTROL_COUNT   2

#define MOTOR_DEAD_VAL_L  0
#define MOTOR_DEAD_VAL_R  0

#define MOTOR_MAX_Z  9600
#define MOTOR_MIN_Z  0

#define MOTOR_MAX_F  0
#define MOTOR_MIN_F  -9600

#define MOTOR_MAX_SPEED 6000
#define MOTOR_MIN_SPEED -6000

void Kalman_Cal(float Gyro,float Accel);
int16 SpeedGet(void);
void Get_Angle(void);
void AngleControl(void);
void SpeedControl(void);
void DirectionControl(void);
void DirectionControlOutput(void);
void MotorOutput(void);
void SetMotorVoltage(void);
float CF_Factor_Cal(float deltaT, float tau);
float CF_1st(float gyroData, float accData, float cf_factor);
float CF_1st_EX(float oldData,float gyroData, float accData, float cf_factor,float deltaT);
//float Complementary_Filtering(float AX,float GY);

extern float AngleZ;
#endif