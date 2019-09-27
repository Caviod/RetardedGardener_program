/******************************(C) COPYRIGHT 2019 SMU智能车实验室****************************
  *     @file           pid.c
  *     @brief          应用结构体指针的pid函数库
  *     @note
  *     @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2019.3.16      MIN                Finish
  ****************************(C) COPYRIGHT 2019 SMU智能车实验室*****************************/
#include "pid.h"


void LimitMax(float32_t input,float32_t max)
{
       if (input > max)
       {
	      input = max;
       }
       else if (input < -max)
       {
	      input = -max;
       }
}


void PID_Init(PidTypeDef *pid, uint8_t mode, const float32_t PID[3], float32_t max_out, float32_t max_iout)
{
       if (pid == NULL || PID == NULL){return;}

       pid->mode = mode;
       pid->Kp = PID[0];
       pid->Ki = PID[1];
       pid->Kd = PID[2];
       pid->max_out = max_out;
       pid->max_iout = max_iout;
       pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
       pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}


float32_t PID_Calc(PidTypeDef  *pid, float32_t ref, float32_t set)
{
       if (pid == NULL){return 0.0f; }

       pid->error[2] = pid->error[1];
       pid->error[1] = pid->error[0];
       pid->set = set;
       pid->fdb = ref;
       pid->error[0] = pid->set - pid->fdb;


       if (pid->mode == PID_POSITION)              
       {
//	       if(pid->error[0]<=1&&pid->error[0]>=-1)
//	       {
//		   return 0;
//	       }
	      pid->Pout = pid->Kp * pid->error[0];
	      pid->Iout += pid->Ki * pid->error[0];
	      pid->Dbuf[2] = pid->Dbuf[1];
	      pid->Dbuf[1] = pid->Dbuf[0];
	      pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
	      pid->Dout = pid->Kd * pid->Dbuf[0];
	      if (pid->Iout > pid->max_iout)
	      {
		     pid->Iout = pid->max_iout;
	      }
	      else if (pid->Iout < -pid->max_iout)
	      {
		     pid->Iout = -pid->max_iout;
	      }
	      pid->out = pid->Pout + pid->Iout + pid->Dout;
	      if (pid->out > pid->max_out)
	      {
		     pid->out = pid->max_out;
	      }
	      else if (pid->out < -pid->max_out)
	      {
		     pid->out = -pid->max_out;
	      }
       }
       else if (pid->mode == PID_DELTA)
       {
	      pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
	      pid->Iout = pid->Ki * pid->error[0];
	      pid->Dbuf[2] = pid->Dbuf[1];
	      pid->Dbuf[1] = pid->Dbuf[0];
	      pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
	      pid->Dout = pid->Kd * pid->Dbuf[0];
	      pid->out += pid->Pout + pid->Iout + pid->Dout;
	      if (pid->out > pid->max_out)
	      {
		     pid->out = pid->max_out;
	      }
	      else if (pid->out < -pid->max_out)
	      {
		     pid->out = -pid->max_out;
	      }
       }
       else if (pid->mode == PD_WithGYRO)
       {
              if(pid->error[0]<1&&pid->error[0]>-1)
	      {
		   return 0;
	      }
	      pid->Pout = pid->Kp * pid->Kp * pid->error[0];
	      pid->Dbuf[0] = pid->ExternalDifferential;
	      pid->Dout = pid->Kd * pid->Dbuf[0];
	      pid->out = pid->Pout + pid->Iout + pid->Dout;
	      if (pid->out > pid->max_out)
	      {
		     pid->out = pid->max_out;
	      }
	      else if (pid->out < -pid->max_out)
	      {
		     pid->out = -pid->max_out;
	      }
       }
       else if (pid->mode ==PID_PosDel)
       {
	      if(pid->error[0]<20&&pid->error[0]>-20)
	      {
		     return 0;
	      }
	      if(pid->error[0]>1000)
	      {
		    pid->tempout=3000;
		    pid->out=0;
		    return pid->tempout;
	      }
	      else if(pid->error[0]<-1000)
	      {
		    pid->tempout=-3000;
		    pid->out=0;
		    return pid->tempout;
	      }
	      else
	      {
		     pid->Pout = pid->Kp * pid->error[0];
		     pid->Iout += pid->Ki * pid->error[0];
		     pid->Dbuf[2] = pid->Dbuf[1];
		     pid->Dbuf[1] = pid->Dbuf[0];
		     pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
		     pid->Dout = pid->Kd * pid->Dbuf[0];

		     if (pid->Iout>pid->max_iout)
		     {
			    pid->Iout = pid->max_iout;
		     }
		     else if (pid->Iout<-pid->max_iout)
		     {
			    pid->Iout = -pid->max_iout;
		     }
		     pid->out+=pid->Pout + pid->Iout + pid->Dout;
		     if (pid->out > pid->max_out)
		     {
			    pid->out = pid->max_out;
		     }
		     else if (pid->out < -pid->max_out)
		     {
			    pid->out = -pid->max_out;
		     }
	      }
       }
       else if (pid->mode ==PID_chassis)
       {
	      if(pid->error[0]<20&&pid->error[0]>-20)
	      {
		     return 0;
	      }
	      else
	      {
		     pid->Pout = pid->Kp * pid->error[0];
		     pid->Iout += pid->Ki * pid->error[0];
		     pid->Dbuf[2] = pid->Dbuf[1];
		     pid->Dbuf[1] = pid->Dbuf[0];
		     pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
		     pid->Dout = pid->Kd * pid->Dbuf[0];

		     if (pid->Iout>pid->max_iout)
		     {
			    pid->Iout = pid->max_iout;
		     }
		     else if (pid->Iout<-pid->max_iout)
		     {
			    pid->Iout = -pid->max_iout;
		     }
		     pid->out=pid->Pout + pid->Iout + pid->Dout;
		     if (pid->out > pid->max_out)
		     {
			    pid->out = pid->max_out;
		     }
		     else if (pid->out < -pid->max_out)
		     {
			    pid->out = -pid->max_out;
		     }
	      }
       }

       return pid->out;
}


void PID_clear(PidTypeDef *pid)
{
       if (pid == NULL)
       {
	      return;
       }
       pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
       pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
       pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
       pid->fdb = pid->set = 0.0f;
}

