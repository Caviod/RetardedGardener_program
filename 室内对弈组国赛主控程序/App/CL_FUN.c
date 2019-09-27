/*********************************************************************************************************************
 * @file       		FUN
 * @company
 * @author
 * @version
 * @Software
 * @Target core
 * @date
 ********************************************************************************************************************/



#include "CL_FUN.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      绝对值函数
//  @param      dat				需要求绝对值的数
//  @return     int				返回绝对值
//  @since      v1.0
//  Sample usage:				dat = myabs(dat);//将dat变成正数
//-------------------------------------------------------------------------------------------------------------------
int  myabs(int dat)
{
    if(dat>=0)  return dat;
    else        return -dat;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      软件延时
//  @param      t				延时时间
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void my_delay(long t)
{
    while(t--);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      限幅
//  @param      x				被限幅的数据
//  @param      y				限幅范围(数据会被限制在-y至+y之间)
//  @return     float			限幅之后的数据
//  @since      v1.0
//  Sample usage:				float dat = limit(500,300);//数据被限制在-300至+300之间  因此返回的结果是300
//-------------------------------------------------------------------------------------------------------------------
float limit(float x, uint16 y)
{
    if(x>y)             return y;
    else if(x<-y)       return -y;
    else                return x;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      双边限幅
//  @param      x				被限幅的数据
//  @param      a				限幅范围左边界
//  @param      b				限幅范围右边界
//  @return     float			限幅之后的数据
//  @since      v1.0
//  Sample usage:				int16 dat = limit_ab(500,-300，400);//数据被限制在-300至+400之间  因此返回的结果是400
//-------------------------------------------------------------------------------------------------------------------
int16 limit_ab(int16 x, int16 a, int16 b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return x;
}

/*四舍五入函数*/
int Round(float fdx)
{
  float T_fdx;
  int res;
  if(fdx>=0)
  {
    T_fdx=fdx-(int)(fdx);
    if((T_fdx-0.5)>=0)
    {res=(int)(fdx)+1;}
    else
      res=(int)(fdx);
  }
  else
  {
    fdx=-fdx;
    T_fdx=fdx-(int)(fdx);
    if((T_fdx-0.5)>=0)
    {res=(int)(fdx)+1;}
    else
      res=(int)(fdx);
    res=-res;
  }
  return res;
}

float Float_ABS(float x)
{
       if(x<0)
	      return -x;
       else
	      return x;
}
//-------------------------------------------------------------------------//
/******** 限幅保护 *********/
float float_range_protect( float duty, float min, float max ) //限幅保护
{
    if( duty > max )
    {
        return max;
    }
    else if( duty <= min )
    {
        return min;
    }
    else
    {
        return duty;
    }
}

/******** 限幅保护 *********/
int32 int32_range_protect( int32  duty, int32  min, int32  max ) //限幅保护
{
    if( duty > max )
    {
        return max;
    }
    else if( duty <= min )
    {
        return min;
    }
    else
    {
        return duty;
    }
}


/******** 限幅保护 *********/
int16 int16_range_protect( int16  duty, int16  min, int16  max ) //限幅保护
{
    if( duty > max )
    {
        return max;
    }
    else if( duty <= min )
    {
        return min;
    }
    else
    {
        return duty;
    }
}

