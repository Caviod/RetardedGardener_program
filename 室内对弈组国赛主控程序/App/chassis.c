/******************************(C) COPYRIGHT 2019 SMU智能车****************************
  *     @file           chassis.c
  *     @brief          实现底盘的控制
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V1.0.1     2019.6.8        min               debug
  ****************************(C) COPYRIGHT 2019 SMU智能车*****************************/
#include "chassis.h"
#include "include.h"
#include <math.h>

Chassis_t  Chassis_Motion;
PidTypeDef Chassis_PID[4];
PidTypeDef Angle_PID[10];
PidTypeDef X_PID[3];
PidTypeDef Y_PID[2];


//PidTypeDef FrontLine_PID;
//PidTypeDef RightLine_PID;


//速度环PI控制  不要D
float32_t ChassisPIDKpList[4] = { Chassis_Kp,Chassis_Kp,Chassis_Kp,Chassis_Kp};
float32_t ChassisPIDKdList[4] = { 0 };
float32_t ChassisPIDKiList[4] = { Chassis_Ki, Chassis_Ki,Chassis_Ki, Chassis_Ki};

//const float32_t AnglePID[3] =
//{
//       150,
//       0,//0.01
//       -16//280 -4
//};     陀螺仪参数

const float32_t AnglePID1[3] =
{
       60,
       0.1,//0.01
       40//280 -4
};
const float32_t AnglePID2[3] =
{
       160,
       0,//0.01
       50//280 -4
};
const float32_t AnglePID3[3] =
{
       120,
       0.1,//0.01
       50//280 -4
};
const float32_t AnglePID4[3] =
{
       160,
       0,//0.01
       50//280 -4
};

const float32_t AnglePID5[3] =
{
       60,
       0.11,//0.01
       50//280 -4
};

const float32_t AnglePID7[3] =
{
       100,
       0,//0.01
       40//280 -4
};




const float32_t XPID[3] =
{
       150,//110
       0.1,//0.02
       50//280 40
};

const float32_t XPID2[3] =
{
       150,//110
       0.02,
       50//280 40
};

const float32_t XPID3[3] =
{
	   20000,
	   0,
	   43000//150000
};


const float32_t YPID[3] =
{
       6000,//110
       0.02,//0.02
       80//280 40
};

const float32_t YPID2[3] =
{
       18000,
       0,
       43000//150000
};


const float32_t Direction_AnglePID[3] =
{
	13000,
	0,
	5000
};


/*外部调用函数*/
void chassis_init(void);                                                                 //底盘初始化
void chassis(uint8 ChassisMode,int CSPEED);                                              //底盘控制的总函数
void chassis_control(Chassis_t *Chassis_Control,int Vx,int Vy,int Wz);                   //底盘控制函数

/*内部调用函数*/
static void chassis_loop_control(Chassis_t *Chassis_Loop);                               //底盘闭环控制函数（内部调用）
void chassis_analyze(int vx,int vy,int wz,int Wheel_Speed[4]);                           //麦克纳姆算解
void chassis_inverse(int *vx, int *vy, int *wz, int Wheel_Speed[]);                      //麦克纳姆逆算解

void   AngleLoop_init();
void   XLoop_init();
void   YLoop_init();
int Angle_Control(Enum_AngleMode Angleloop_mode,float eeeerrr,float AngleSet);
int    X_Control(int eeeerrr,float XSet);//AngleSet度角闭环程序
int    X_Control2(int eeeerrr,float XSet);//AngleSet度角闭环程序
int    X_Control3(float eeeerrr, float XSet);//AngleSet度角闭环程序
int    Y_Control(int eeeerrr,float YSet);//AngleSet度角闭环程序
int    Y_Control2(float eeeerrr,float YSet);//AngleSet度角闭环程序

int AngleControlValue = 0;//偏航角闭环控制量
int XControlValue = 0;//偏航角闭环控制量
int YControlValue = 0;



ControlMode  NowMode = Chassis_Stable;

float dist_to_target;
/***********************************************************
底盘控制总函数
* 函数名称： chassis(uint8 ChassisMode)
* 功能说明： 底盘控制
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
************************************************************/
void chassis(uint8 ChassisMode,int CSPEED)
{
  float lam = 0;
  //chassis_data_update(&Chassis_Motion);                                  //更新底盘数据
  //AngleControlValue =-Angle_Control(MVerr_loop,1);
  //XControlValue= X_Control(0);
  //FrontLineValue = FrontLine_Control(37);//37
  switch(ChassisMode)
  {
  case Chassis_Stable:                                                //底盘稳定模式
    Chassis_CAN_Cmd(0,0,0,0)         ;                                //给四个电机发送停止的指令
    break;
    
  case DA_Loop:
    {
      float error_angle = PlannerError_Origin.target_angle - (DRw + CORRw);
      while(error_angle > +PI) {
        error_angle -= 2*PI;
      }
      while(error_angle < -PI) {
        error_angle += 2*PI;
      }
      float error_x = PlannerError_Origin.target_x - (DRx + CORRx);
      float error_y = PlannerError_Origin.target_y - (DRy + CORRy);
      dist_to_target = sqrt(error_x*error_x + error_y*error_y);
      AngleControlValue =-Angle_Control(Direction_Angle_Loop, error_angle, 0);
      XControlValue = (int)(lam * XControlValue + (1-lam) * -X_Control3(error_x, 0));
      YControlValue = (int)(lam * YControlValue + (1-lam) * -Y_Control2(error_y, 0));
      chassis_control(&Chassis_Motion, 0, speed_max, AngleControlValue);
      break;
    }
    
  case Chassis_Remote:
    RemoteControl(0);
    break;
    
  }
}


/***********************************************************
底盘初始化

* 函数名称： chassis_init()
* 功能说明： 底盘初始化
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
************************************************************/
void chassis_init()
{
       float32_t PIDPara[3];
       for (int i = 0; i < 4; i++)
       {
	      PIDPara[0] = ChassisPIDKpList[i];
	      PIDPara[1] = ChassisPIDKiList[i];
	      PIDPara[2] = ChassisPIDKdList[i];
	      PID_Init(&Chassis_PID[i], PID_chassis, PIDPara, Chassis_PID_Maxout, Chassis_PID_Maxiout);           //底盘PID初始化
       }
       CAN_Init();  //电机can通信初始化
       AngleLoop_init();
       XLoop_init();
       YLoop_init();
}


float rv=0.0;
int Xmax,Ymax;
int speed_max=0;
/***********************************************************
底盘控制函数

* 函数名称： chassis_control(Chassis_t *Chassis_Control, float32_t Vx, float32_t Vy, float32_t Wz)
* 功能说明： 底盘控制函数
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
************************************************************/
void chassis_control(Chassis_t *Chassis_Control, int Vx, int Vy, int Wz)
{
  rv=sqrt(Vx*Vx+Vy*Vy);
#if 0
  if(dist_to_target<1)
    speed_max=2500;
  else
    speed_max=2500.0+dist_to_target*0.0;
#endif     
      
       Xmax=(int)(speed_max*ABS(Vx)/rv*1.0);
       Ymax=(int)(speed_max*ABS(Vy)/rv*1.0);
//       Vx=int16_range_protect(Vx, -Xmax, Xmax ) ;
//       Vy=int16_range_protect(Vy, -Ymax, Ymax ) ;
       chassis_analyze(Vx,Vy,Wz,Chassis_Control->Chassis_Speed);   //对麦克纳姆轮进行算解转化为四个电机的值
       chassis_loop_control(Chassis_Control);
       Chassis_CAN_Cmd(Chassis_Control->Chassis_Give_Current[0],Chassis_Control->Chassis_Give_Current[1],Chassis_Control->Chassis_Give_Current[2],Chassis_Control->Chassis_Give_Current[3]);

}


/***********************************************************
底盘闭环控制函数

* 函数名称：chassis_control_loop(Chassis_t *Chassis_Loop)
* 功能说明： 底盘闭环控制函数
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
************************************************************/
void chassis_loop_control(Chassis_t *Chassis_Loop)
{
       for(uint8 i=0;i<4;i++)                                                                                        //对四个轮子给值进行赋值
       {
	      int16_range_protect(Chassis_Loop->Chassis_Speed[i], -19000, 19000);
	      Chassis_Loop->Chassis_Give_Current[i]=(int)PID_Calc(&Chassis_PID[i], MotorData[i].Rpm, Chassis_Loop->Chassis_Speed[i]);
       }
}


//对底盘进行麦轮算解,将轮子xyz三个方向上的矢量算解成四个轮子的量
void chassis_analyze(int vx, int vy, int wz, int Wheel_Speed[4])//600, 500, 450
{
       Wheel_Speed[0]=(vx+vy-wz);
       Wheel_Speed[1]=-(vx-vy+wz);
       Wheel_Speed[2]=-(vx+vy+wz);
       Wheel_Speed[3]=(vx-vy-wz);
}

#if 0
//从麦轮的速度反向解算出三个分量（仅使用前三个轮的速度）
void chassis_inverse(int *vx, int *vy, int *wz, int Wheel_Speed[])
{
       int w0 = Wheel_Speed[0];
       int w1 = Wheel_Speed[1];
       int w2 = Wheel_Speed[2];
       int w3 = Wheel_Speed[3];

       *vx =(int)(0.5*w0 - 0.5*w1);
       *vy =(int)(0.5*w0 - 0.5*w3);
       *wz =(int)(-0.5*w1 -0.5*w3);

//       *vx =(int)( 0.25*(w0-w1+w3-w2));
//       *vy =(int)( 0.25*(w0-w3+w1-w2));
//       *wz =(int)(-0.25*(w0+w1+w3+w2));
}
#endif

//从麦轮的速度反向解算出三个分量（仅使用前三个轮的速度）
void chassis_inverse(int *vx, int *vy, int *wz, int Wheel_Speed[])
{
       int w0 = Wheel_Speed[0];
       int w1 = Wheel_Speed[1];
       int w2 = Wheel_Speed[2];
       int w3 = Wheel_Speed[3];

       /*
       float vx1 =(int)(0.5*w0 - 0.5*w1);
       float vy1 =(int)(0.5*w0 - 0.5*w3);
       float wz1 =(int)(-0.5*w1 -0.5*w3);

       float vx2 =(int)(-0.5*w2 + 0.5*w3);
       float vy2 =(int)(0.5*w1 - 0.5*w2);
       float wz2 =(int)(-0.5*w1 -0.5*w3);

       float vx3 =(int)(-0.5*w2 + 0.5*w3);
       float vy3 =(int)(0.5*w0 - 0.5*w3);
       float wz3 =(int)(-0.5*w0 -0.5*w2);
       */

       *vx =(int)( 0.5*(+w0-w1-w2+w3) / 2 );
       *vy =(int)( 0.5*(+w0+w1-w2-w3) / 2 );
       *wz =(int)( 0.5*(-w0-w1-w2-w3) / 2 );
}


//偏航角度闭环初始化
void AngleLoop_init()
{
       PID_Init(&Angle_PID[0], PID_POSITION, AnglePID1, Angle_PID_Maxout, Angle_PID_Maxiout);
       PID_Init(&Angle_PID[1], PID_POSITION, AnglePID2, Angle_PID_Maxout, Angle_PID_Maxiout);
       PID_Init(&Angle_PID[2], PID_POSITION, AnglePID3, Angle_PID_Maxout, Angle_PID_Maxiout);
       PID_Init(&Angle_PID[3], PID_POSITION, AnglePID4, Angle_PID_Maxout, Angle_PID_Maxiout);
       PID_Init(&Angle_PID[4], PID_POSITION, AnglePID5, Angle_PID_Maxout, Angle_PID_Maxiout);
       PID_Init(&Angle_PID[5], PID_POSITION, AnglePID7, Angle_PID_Maxout, Angle_PID_Maxiout);
       PID_Init(&Angle_PID[6], PID_POSITION, Direction_AnglePID, Angle_PID_Maxout, Angle_PID_Maxiout);
}

int Angle_Control(Enum_AngleMode Angleloop_mode,float eeeerrr,float AngleSet)//AngleSet度角闭环程序
{
       switch(Angleloop_mode)
       {
	    case MVerr_loop:                                                          //底盘稳定模式
	      {
		     return (int)PID_Calc(&Angle_PID[0],eeeerrr, AngleSet);
		     break;
	      }
	    case MVCurl_LOOP:                                                          //遥控模式
	      {
		     return (int)PID_Calc(&Angle_PID[1],eeeerrr, AngleSet);
		     break;
	      }
	    case Cemian_MVCurl_LOOP:                                                          //底盘稳定模式
	      {
		     return (int)PID_Calc(&Angle_PID[2],eeeerrr, AngleSet);
		     break;
	      }
	    case Yaw_Loop:                                                          //遥控模式
	      {
                return (int)PID_Calc(&Angle_PID[3],eeeerrr, AngleSet);
                break;
	      }
        case  Cemian_MVerr_LOOP:
	  {
		 return (int)PID_Calc(&Angle_PID[4],eeeerrr, AngleSet);
		 break;
	  }
	    case  Q_MVCurl_LOOP:
	      {
		     return (int)PID_Calc(&Angle_PID[5],eeeerrr, AngleSet);
		     break;
	      }
	    case  Direction_Angle_Loop:
	      {
		     return (int)PID_Calc(&Angle_PID[6], eeeerrr, AngleSet);
		     break;
	      }
       }
      return 0;
}


//x方向闭环初始化
void XLoop_init()
{
    PID_Init(&X_PID[0], PID_POSITION, XPID, Chassis_PID_Maxout, Angle_PID_Maxiout);
    PID_Init(&X_PID[1], PID_POSITION, XPID2, Chassis_PID_Maxout, Angle_PID_Maxiout);
    PID_Init(&X_PID[2], PID_POSITION, XPID3, Chassis_PID_Maxout, Angle_PID_Maxiout);
}

int X_Control(int eeeerrr,float XSet)//AngleSet度角闭环程序
{
     //Angle_PID.ExternalDifferential=stcGyro.w[2];
     //return PID_Calc(&Angle_PID, yaw, AngleSet);
       return (int)PID_Calc(&X_PID[0], eeeerrr, XSet);
}
int X_Control2(int eeeerrr,float XSet)//AngleSet度角闭环程序
{
     //Angle_PID.ExternalDifferential=stcGyro.w[2];
     //return PID_Calc(&Angle_PID, yaw, AngleSet);
       return (int)PID_Calc(&X_PID[1], eeeerrr, XSet);
}

int X_Control3(float eeeerrr, float XSet)//AngleSet度角闭环程序
{
	//Angle_PID.ExternalDifferential=stcGyro.w[2];
	//return PID_Calc(&Angle_PID, yaw, AngleSet);
	return (int)PID_Calc(&X_PID[2], eeeerrr, XSet);
}

//Y方向闭环初始化
void YLoop_init()
{
    PID_Init(&Y_PID[0], PID_POSITION, YPID, Chassis_PID_Maxout, Angle_PID_Maxiout);
    PID_Init(&Y_PID[1], PID_POSITION, YPID2, Chassis_PID_Maxout, Angle_PID_Maxiout);
}

int Y_Control(int eeeerrr,float YSet)//AngleSet度角闭环程序
{
     //Angle_PID.ExternalDifferential=stcGyro.w[2];
     //return PID_Calc(&Angle_PID, yaw, AngleSet);
       return (int)PID_Calc(&Y_PID[0], eeeerrr, YSet);
}
int Y_Control2(float eeeerrr,float YSet)//AngleSet度角闭环程序
{
     //Angle_PID.ExternalDifferential=stcGyro.w[2];
     //return PID_Calc(&Angle_PID, yaw, AngleSet);
       return (int)PID_Calc(&Y_PID[1], eeeerrr, YSet);
}
void RemoteControl(float RemoteWz)
{
       float RemoteVx = 0, RemoteVy = 0;
       if(RemoteData.Left_X < 124 || RemoteData.Left_X > 131)
	      RemoteVx = (RemoteData.Left_X-127) * 40;
       else
	      RemoteVx = 0;
       if(RemoteData.Left_Y < 124 || RemoteData.Left_Y > 131)
	      RemoteVy = (RemoteData.Left_Y-127) * 40;
       else
	      RemoteVy = 0;

       if(RemoteData.Right_X < 124 || RemoteData.Right_X > 131)
	      RemoteData.Right_X = RemoteData.Right_X;
       else
	      RemoteData.Right_X=127;
       if(RemoteData.Right_Y < 124 || RemoteData.Right_Y > 131)
	      RemoteData.Right_Y = RemoteData.Right_Y;
       else
	      RemoteData.Right_Y=127;

       chassis_control(&Chassis_Motion,(int) RemoteVx, (int) RemoteVy, (int) RemoteWz);
}
