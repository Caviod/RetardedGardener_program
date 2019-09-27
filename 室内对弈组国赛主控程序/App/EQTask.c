#include "include.h"
#include "EQTask.h"
unsigned char NowGrid = 0;
_EQ_Task EQ_TaskPool[60];//任务池
unsigned char TaskIndex = 0;//任务进程序列
//extern int Qopenmv.Block_Count;
//extern int Yopenmv.Block_Count;
unsigned long Count_StopMode = 0;
unsigned char Flag_YuandiXuanzhuan = 0;
void DoTask()
{
    /*更新任务状态*/
    RenewTaskProcess(&EQ_TaskPool[TaskIndex]);
    /*判断任务是否结束*/
    if (EQ_TaskPool[TaskIndex].TaskProcess >= EQ_TaskPool[TaskIndex].TaskPara)//任务结束条件
    {
        TaskIndex++;
    }
    /*根据任务指令执行对应操作*/
    switch (EQ_TaskPool[TaskIndex].TaskCommand)
    {
    case Forward:
      {
        chassis(Q_MidX_Z_LOOP,3000);
        break;
      }
    case Back:
      {
        chassis(Y_Curl_Z_loop,-3000);
        break;
      }
    case Left:
      {
        chassis(Q_Curl_Z_loop,-3000);
        break;
      }
    case Right:
    {
                chassis(Y_Mid_Z_LOOP,4000);
                  break;
    }
    case HoldChess:
    {
      if(!Flag_Holding)  
      {
        StartHoldChess();
        Flag_Holding = 1;
      }
      chassis(Chassis_Stable,0);
                 break;
    }
    case DownChess:
    {
      if(!Flag_Downing)  
      {
        StartDownChess();
        Flag_Downing = 1;
      }
      chassis(Chassis_Stable,0);
                 break;
    }
    case Stop:
    {
      if(TaskIndex >= 1)
      {
        if(EQ_TaskPool[TaskIndex - 1].TaskCommand == Forward || EQ_TaskPool[TaskIndex - 1].TaskCommand == Back)
        {
          chassis(QZ_CH_CX_loop,0);
        } 
        else if(EQ_TaskPool[TaskIndex - 1].TaskCommand == Left || EQ_TaskPool[TaskIndex - 1].TaskCommand == Right)
        {
          chassis(QZ_CH_CX_loop2,0);
        }
      }
     
      //SEND(AngleControlValue, XControlValue, YControlValue, TaskIndex);
                 break;
    }
    case SpecialStop:
    {
       chassis(QZ_CH_CX_loop333,0);
      break;
    }
    case SpecialForward:
      {
        chassis(Y_Curl_Z_loop,3000);
        break;
      }
    case YuandiXuanzhuan_You:
      {
//        chassis_control(&Chassis_Motion,0,0,-2000);     
        break;
      }
    default:
        chassis(Chassis_Stable,0);
        break;
    }

}
void DoTaskEx(uint8 action)
{
    /*根据任务指令执行对应操作*/
    switch (action) {
    case 1:
	{
            if (!Flag_Holding)
            {
                StartHoldChess();
                Flag_Holding = 1;
            }
            break;
	}
    case 0:
	{
            if (!Flag_Downing)
            {
                StartDownChess();
                Flag_Downing = 1;
            }
            break;
	}
    }
}
extern unsigned char Flag_Holding;
extern unsigned char Flag_Downing;
int LastQopenmvBlock_Count = 0;
int LastYopenmvBlock_Count = 0;
extern unsigned char Flag_StartPressOnce;

//更新任务进度函数
void RenewTaskProcess(_EQ_Task * NowTask)
{
       int GridCha = 0;
       switch (NowTask->TaskCommand)
       {
	    case Forward://进度改变数即为格子变化数
	      {
		     GridCha = Yopenmv.Block_Count - LastYopenmvBlock_Count;
		     NowTask->TaskProcess += GridCha;
//		     if(Flag_StartPressOnce)
//                     {
//                        NowTask->TaskProcess = NowTask->TaskPara;   
//                        Flag_StartPressOnce = 0;
//                     }
                     break;
	      }
	    case Back://进度改变数即为格子变化数
	      {
		     GridCha = Yopenmv.Block_Count - LastYopenmvBlock_Count;
		     NowTask->TaskProcess += -GridCha;
//		     if(Flag_StartPressOnce)
//                     {
//                        NowTask->TaskProcess = NowTask->TaskPara;   
//                        Flag_StartPressOnce = 0;
//                     }
                     break;
	      }
	    case Left://进度改变数即为格子变化数
	      {
                      GridCha = Qopenmv.Block_Count - LastQopenmvBlock_Count;
		     
		     NowTask->TaskProcess += GridCha;
//		    if(Flag_StartPressOnce)
//                     {
//                        NowTask->TaskProcess = NowTask->TaskPara;   
//                        Flag_StartPressOnce = 0;
//                     }
                      break;
	      }
	    case Right://进度改变数即为格子变化数
	      {
                    GridCha = Qopenmv.Block_Count - LastQopenmvBlock_Count;
		     
		     NowTask->TaskProcess += -GridCha;
//		    if(Flag_StartPressOnce)
//                     {
//                        NowTask->TaskProcess = NowTask->TaskPara;   
//                        Flag_StartPressOnce = 0;
//                     }
                     break;
	      }
	    case HoldChess://吸棋子动作结束则满进度
	      {
		     if (Flag_Holding)
		     {
			    if (Check_HoldOrDownFinish())
			    {
				   Flag_Holding = 0;
				   NowTask->TaskProcess = NowTask->TaskPara;
			    }
		     }
		     break;
	      }
	    case DownChess://放棋子动作结束则满进度
	      {
		     if (Flag_Downing)
		     {
			    if (Check_HoldOrDownFinish())
			    {
				   Flag_Downing = 0;
				   NowTask->TaskProcess = NowTask->TaskPara;
			    }
		     }
		     break;
	      }
	    case Stop://停车判断条件达成则满进度
	      {
                      Count_StopMode++;
		     if (ABS(AngleControlValue)<=1&&ABS(XControlValue)<400 &&ABS(YControlValue) <= 1)
		     {
                          
                          //if(Count_StopMode >= 5)
                          //{
                              NowTask->TaskProcess = NowTask->TaskPara;
                          //}
                              Count_StopMode = 0;
                            
                     }
                     if(Count_StopMode >= 100)
                     {
                              NowTask->TaskProcess = NowTask->TaskPara;
                              Count_StopMode = 0;                       
                     }
                  break;
    }
       case SpecialStop:
         {
                      Count_StopMode++;
		     if (ABS(AngleControlValue)<=1&&ABS(XControlValue)<400 &&ABS(YControlValue) <= 1)
		     {
                          
                          //if(Count_StopMode >= 5)
                          //{
                              NowTask->TaskProcess = NowTask->TaskPara;
                          //}
                              Count_StopMode = 0;
                            
                     }
                     if(Count_StopMode >= 100)
                     {
                              NowTask->TaskProcess = NowTask->TaskPara;
                              Count_StopMode = 0;                       
                     }
                  break;           
         }
             case SpecialForward:
      {
		     GridCha = Yopenmv.Block_Count - LastYopenmvBlock_Count;
		     NowTask->TaskProcess += GridCha;
//		     if(Flag_StartPressOnce)
//                     {
//                        NowTask->TaskProcess = NowTask->TaskPara;   
//                        Flag_StartPressOnce = 0;
//                     }
                     break;
      }
       case YuandiXuanzhuan_You:
         {
             if(ABS(Qopenmv.Mid_X) > 20)
             {
                       NowTask->TaskProcess = NowTask->TaskPara;               
             }
           break;
         }
    default:

        break;
    }
    LastQopenmvBlock_Count = Qopenmv.Block_Count;
    LastYopenmvBlock_Count = Yopenmv.Block_Count;
}

void ConfigTask(_EQ_Task * NowTask, _EQ_RunCMD CMD, int Para)
{
    switch (CMD)//根据车模进入方向更改方向指令
    {
    //case Forward:
    //{
    //                NowTask->TaskCommand = Right;
    //                break;
    //}
    //case Back:
    //{
    //             NowTask->TaskCommand = Left;
    //             break;
    //}
    //case Left:
    //{
    //             NowTask->TaskCommand = Forward;
    //             break;
    //}
    //case Right:
    //{
    //              NowTask->TaskCommand = Back;
    //              break;
    //}
    default:
        NowTask->TaskCommand = CMD;
        break;
    }
    
    NowTask->TaskPara = Para;
}


void CreateTaskPool()
{
    ConfigTask(&EQ_TaskPool[0], Forward, 1);//1
    ConfigTask(&EQ_TaskPool[1], Stop, 10);
    ConfigTask(&EQ_TaskPool[2], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[3], Forward, 2);
    ConfigTask(&EQ_TaskPool[4], Stop, 10);
    ConfigTask(&EQ_TaskPool[5], DownChess, 10);

    ConfigTask(&EQ_TaskPool[6], Right, 1);
    ConfigTask(&EQ_TaskPool[7], Stop, 10);
    ConfigTask(&EQ_TaskPool[8], Back, 1);
    ConfigTask(&EQ_TaskPool[9], Stop, 10);
    ConfigTask(&EQ_TaskPool[10], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[11], Right, 1);
    ConfigTask(&EQ_TaskPool[12], Stop, 10);
    ConfigTask(&EQ_TaskPool[13], DownChess, 10);

    ConfigTask(&EQ_TaskPool[14], Forward, 1);
    ConfigTask(&EQ_TaskPool[15], Stop, 10);
    ConfigTask(&EQ_TaskPool[16], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[17], Left, 1);
    ConfigTask(&EQ_TaskPool[18], Stop, 10);
    ConfigTask(&EQ_TaskPool[19], Forward, 2);
    ConfigTask(&EQ_TaskPool[20], Stop, 10);
    ConfigTask(&EQ_TaskPool[21], DownChess, 10);

    ConfigTask(&EQ_TaskPool[22], Right, 2);
    ConfigTask(&EQ_TaskPool[23], Stop, 10);
    ConfigTask(&EQ_TaskPool[24], Back, 1);
    ConfigTask(&EQ_TaskPool[25], Stop, 10);
    ConfigTask(&EQ_TaskPool[26], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[27], Right, 2);
    ConfigTask(&EQ_TaskPool[28], Stop, 10);
    ConfigTask(&EQ_TaskPool[29], DownChess, 10);

    ConfigTask(&EQ_TaskPool[30], Forward, 1);
    ConfigTask(&EQ_TaskPool[31], Stop, 10);
    ConfigTask(&EQ_TaskPool[32], Left, 1);
    ConfigTask(&EQ_TaskPool[33], Stop, 10);
    ConfigTask(&EQ_TaskPool[34], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[35], Forward, 1);
    ConfigTask(&EQ_TaskPool[36], Stop, 10);
    ConfigTask(&EQ_TaskPool[37], DownChess, 10);

    ConfigTask(&EQ_TaskPool[38], Right, 1);
    ConfigTask(&EQ_TaskPool[39], Stop, 10);
    ConfigTask(&EQ_TaskPool[40], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[41], Left, 2);
    ConfigTask(&EQ_TaskPool[42], Stop, 10);
    ConfigTask(&EQ_TaskPool[43], SpecialForward, 2);
    ConfigTask(&EQ_TaskPool[44], SpecialStop, 10);
    ConfigTask(&EQ_TaskPool[45], DownChess, 10);

    ConfigTask(&EQ_TaskPool[46], YuandiXuanzhuan_You, 10);   
    //ConfigTask(&EQ_TaskPool[47], SpecialStop, 10);
    ConfigTask(&EQ_TaskPool[47], SpecialForward, 4);
    ConfigTask(&EQ_TaskPool[48], SpecialStop, 10);
    ConfigTask(&EQ_TaskPool[49], HoldChess, 10);
    ConfigTask(&EQ_TaskPool[50], YuandiXuanzhuan_You, 10); 
    //ConfigTask(&EQ_TaskPool[52], SpecialStop, 10);
    ConfigTask(&EQ_TaskPool[51], SpecialForward, 7);
    ConfigTask(&EQ_TaskPool[52], SpecialStop, 10);
    ConfigTask(&EQ_TaskPool[53], DownChess, 10);

    ConfigTask(&EQ_TaskPool[54], SpecialForward, 1);
    ConfigTask(&EQ_TaskPool[55], NoneCMD, 10);

    
    
    
    
    
    
    
//    ConfigTask(&EQ_TaskPool[46], Right, 4);
//    ConfigTask(&EQ_TaskPool[47], Stop, 10);
//    ConfigTask(&EQ_TaskPool[48], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[49], Back, 7);
//    ConfigTask(&EQ_TaskPool[50], Stop, 10);
//    ConfigTask(&EQ_TaskPool[51], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[52], Back, 1);
//    ConfigTask(&EQ_TaskPool[53], NoneCMD, 10);
}


//void CreateTaskPool()
//{
//    //ConfigTask(&EQ_TaskPool[0], Forward, 1);//1
//    ConfigTask(&EQ_TaskPool[0], Forward, 2);//1
//    ConfigTask(&EQ_TaskPool[1], Stop, 10);
//    ConfigTask(&EQ_TaskPool[2], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[3], Forward, 2);
//    ConfigTask(&EQ_TaskPool[4], Stop, 10);
//    ConfigTask(&EQ_TaskPool[5], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[6], Right, 1);
//    ConfigTask(&EQ_TaskPool[7], Back, 1);
//    ConfigTask(&EQ_TaskPool[8], Stop, 10);
//    ConfigTask(&EQ_TaskPool[9], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[10], Right, 1);
//    ConfigTask(&EQ_TaskPool[11], Stop, 10);
//    ConfigTask(&EQ_TaskPool[12], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[13], Forward, 1);
//    ConfigTask(&EQ_TaskPool[14], Stop, 10);
//    ConfigTask(&EQ_TaskPool[15], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[16], Left, 1);
//    ConfigTask(&EQ_TaskPool[17], Forward, 2);
//    ConfigTask(&EQ_TaskPool[18], Stop, 10);
//    ConfigTask(&EQ_TaskPool[19], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[20], Right, 2);
//    ConfigTask(&EQ_TaskPool[21], Back, 1);
//    ConfigTask(&EQ_TaskPool[22], Stop, 10);
//    ConfigTask(&EQ_TaskPool[23], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[24], Right, 2);
//    ConfigTask(&EQ_TaskPool[25], Stop, 10);
//    ConfigTask(&EQ_TaskPool[26], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[27], Forward, 1);
//    ConfigTask(&EQ_TaskPool[28], Left, 1);
//    ConfigTask(&EQ_TaskPool[29], Stop, 10);
//    ConfigTask(&EQ_TaskPool[30], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[31], Forward, 1);
//    ConfigTask(&EQ_TaskPool[32], Stop, 10);
//    ConfigTask(&EQ_TaskPool[33], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[34], Right, 1);
//    ConfigTask(&EQ_TaskPool[35], Stop, 10);
//    ConfigTask(&EQ_TaskPool[36], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[37], Left, 2);
//    ConfigTask(&EQ_TaskPool[38], Forward, 2);
//    ConfigTask(&EQ_TaskPool[39], Stop, 10);
//    ConfigTask(&EQ_TaskPool[40], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[41], YuandiXuanzhuan_You, 10);   
//    ConfigTask(&EQ_TaskPool[42], Stop, 10);
//    ConfigTask(&EQ_TaskPool[43], Forward, 4);
//    ConfigTask(&EQ_TaskPool[44], Stop, 10);
//    ConfigTask(&EQ_TaskPool[45], HoldChess, 10);
//    ConfigTask(&EQ_TaskPool[46], YuandiXuanzhuan_You, 10); 
//    ConfigTask(&EQ_TaskPool[47], Stop, 10);
//    ConfigTask(&EQ_TaskPool[48], Forward, 7);
//    ConfigTask(&EQ_TaskPool[49], Stop, 10);
//    ConfigTask(&EQ_TaskPool[50], DownChess, 10);
//
//    ConfigTask(&EQ_TaskPool[47], Forward, 1);
//    ConfigTask(&EQ_TaskPool[48], NoneCMD, 10);
//    
////    ConfigTask(&EQ_TaskPool[41], Right, 4);
////    ConfigTask(&EQ_TaskPool[42], Stop, 10);
////    ConfigTask(&EQ_TaskPool[43], HoldChess, 10);
////    ConfigTask(&EQ_TaskPool[44], Back, 7);
////    ConfigTask(&EQ_TaskPool[45], Stop, 10);
////    ConfigTask(&EQ_TaskPool[46], DownChess, 10);
////
////    ConfigTask(&EQ_TaskPool[47], Back, 1);
////    ConfigTask(&EQ_TaskPool[48], Stop, 10);
//}
