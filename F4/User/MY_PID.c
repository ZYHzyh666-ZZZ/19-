#include "MY_PID.h"
#include "Uart_tiva.h"

#define ABS(x) ((x)>0?(x):-(x))

_PID_parameter	PID_HIGH;
_PID_parameter	PID_YAW;

_PID_parameter  PID_1;//高度PID
_PID_parameter  PID_2;
_PID_parameter  PID_3;


#define _LINE_USE_ 0
#define _T265_USE_ 0

#if _LINE_USE_

_PID_parameter	PID_Line_X;
_PID_parameter	PID_Line_Y;
_PID_parameter	PID_DONE_X;
_PID_parameter	PID_DONE_Y;

#endif

#if _T265_USE_
_PID_parameter	PID_T265_X;
_PID_parameter	PID_T265_Y;
_PID_parameter	PID_attack_X;
_PID_parameter	PID_attack_Y;
_PID_parameter	PID_attack_Z;

#endif

/**
  * 函数作用：  初始化PID配置
  * 参数1：     （_PID_parameter）PID名称
  * 参数2：     （float）P值
  * 参数3：     （float）I值
  * 参数4：     （float）D值
  * 参数5：     （float）out_max值
  * 参数6：     （float）out_min值

  * 返回值：     void
  * 
  * 备注：      无
  * 更新日期：   2025-12-7
  */
void _PID_Init(_PID_parameter* PID, float P, float I, float D, float out_max, float out_min)
{
    PID->kp = P;
    PID->ki = I;
    PID->kd = D;

    PID->err_current = 0.0f;
    PID->err_last = 0.0f;
    PID->err_prev = 0.0f;              

    PID->limit_max = out_max;
    PID->limit_min = out_min;
}

/**
  * 函数作用：  传统PID计算
  * 参数1：     （_PID_parameter）PID名称
  * 参数2：     （float）exp目标误差

  * 返回值：     (int）PID输出
  * 
  * 备注：      无
  * 更新日期：   2025-12-7
  *             2026-1-13 修改PID计算位置
  */
int _PID_Traditional_computing(_PID_parameter PID,float exp)//传统PID计算
{
    /*误差计算*/
    PID.err_current = exp;

    /*误差微分*/
    PID.err_prev += PID.err_last;

    // /*误差限幅*/
    // if(ABS(PID.err_current) < 5) 
    //     PID.out = 0;

    /*积分限幅*/
    if (PID.err_prev > 20.0f)
      PID.err_prev = 0;
    else if (PID.err_prev < -20.0f)
      PID.err_prev = 0;

      
    /*PID计算*/
    PID.out = PID.kp * PID.err_current
              + PID.ki * PID.err_prev
              + PID.kd * (PID.err_current - PID.err_last); 

    PID.err_last = PID.err_current;

	return limit_pidout(1.5f,PID.out, PID.limit_max, PID.limit_min);
}

/**
  * 函数作用：  增量式PID计算
  * 参数1：     （_PID_parameter）PID名称
  * 参数2：     （float）exp目标误差

  * 返回值：     (int）PID输出
  * 
  * 备注：      无
  * 更新日期：   2026-1-13
  */
int _PID_Increment_computing(_PID_parameter PID,float exp)//增量式PID计算
{
    /*误差计算*/
    PID.err_current = exp;

    /*误差微分*/
    PID.err_prev += PID.err_last;

    /*误差限幅*/
    if(ABS(PID.err_current) < 5) 
        PID.out = 0;

    /*积分限幅*/
    if (PID.err_prev > 20.0f)
      PID.err_prev = 0;
    else if (PID.err_prev < -20.0f)
      PID.err_prev = 0;

    /*PID计算*/
    PID.out += PID.kp * PID.err_current
              + PID.ki * PID.err_prev
              + PID.kd * (PID.err_current - PID.err_last); 

    PID.err_last = PID.err_current;

	return limit_pidout(1.5f,PID.out, PID.limit_max, PID.limit_min);
}

/**
  * 函数作用：  PID输出限幅
  * 参数1：     （float）增幅倍数
  * 参数2：     （float）PID输出值
  * 参数3：     （float）输出上限
  * 参数4：     （float）输出下限
  * 返回值：    （int）PID输出
  * 
  * 备注：      一般增幅倍数为1.0f
  * 更新日期：   2025-11-27
  */
int limit_pidout(float pid_k, float pidout, float max, float min)
{
    pidout = pid_k * pidout;
    if (pidout > max)
        pidout = max;
    else if (pidout < min)
        pidout = min;   
    return (int)pidout;
}

/*-----------------------------------------------------------------------------------------------*/
/*-------------------------------------应用层----------------------------------------------------*/

/*PID参数初始化*/
void MY_PID_Init(void)
{
  /*定高PID参数初始化*/
    _PID_Init(&PID_HIGH, 0.55f, 0.0f, 0.2f, 40.0f, -40.0f);

  /*航向角PID参数初始化*/  
    _PID_Init(&PID_YAW, 0.008f, 0.0f, 0.001f, 22.0f, -22.0f);
	
  /*PID参数初始化*/
    _PID_Init(&PID_1, 0.02f, 0.0f, 0.01f, 20.0f, -20.0f);  //高度环
    _PID_Init(&PID_2, 0.1f, 0.0f, 0.01f, 22.0f, -22.0f);   //距离环
    _PID_Init(&PID_3, 0.008f, 0.0f, 0.001f, 22.0f, -22.0f);
}



/**
  * 函数作用：  光流闭环速度定高
  * 参数1：     （float）目标高度
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合匿名光流使用
  * 更新日期：   2025-11-27
  */
int PID_calculate_user_high(float exp)//高度PID计算
{
    /*高度数据只取光流高度*/
    PID_HIGH.err_current = exp - ano_of.of_alt_cm;

    /*计算PID*/
	  PID_HIGH.out = PID_HIGH.kp * PID_HIGH.err_current
                 + PID_HIGH.ki * PID_HIGH.err_prev
                 + PID_HIGH.kd * (PID_HIGH.err_current - PID_HIGH.err_last);

    /*误差微分*/
    PID_HIGH.err_last = PID_HIGH.err_current;

    /*误差积分*/
    PID_HIGH.err_prev += PID_HIGH.err_current;

    /*积分限幅*/
    if (PID_HIGH.err_prev > 20.0f)
      PID_HIGH.err_prev = 0;
    if (PID_HIGH.err_prev < -20.0f)
      PID_HIGH.err_prev = 0;
    
    /*降落死区*/
    if(begin_land == 1)
    {
        if(ano_of.of_alt_cm < 100 && ano_of.of_alt_cm >= 60)
        {
            PID_HIGH.limit_max = 15.0f;
            PID_HIGH.limit_min = -15.0f;
        }
        else if(ano_of.of_alt_cm < 60 && ano_of.of_alt_cm >= 35)
        {
            PID_HIGH.limit_max = 25.0f;
            PID_HIGH.limit_min = -20.0f;
        }
        else if(ano_of.of_alt_cm < 35 && ano_of.of_alt_cm > 0)
        {
            PID_HIGH.limit_max = 35.0f;
            PID_HIGH.limit_min = -35.0f;
        }
        else
        {
            PID_HIGH.limit_max = 30.0f;
            PID_HIGH.limit_min = -30.0f;
        }
    }
    else
    {
        PID_HIGH.limit_max = 40.0f;
        PID_HIGH.limit_min = -40.0f;
    }

    return limit_pidout(1.3f,PID_HIGH.out, PID_HIGH.limit_max, PID_HIGH.limit_min);
}

/**
  * 函数作用：  PID1更新函数
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,使用底层传统PID计算函数
  */
int PID1_updata(float exp)
{
  return _PID_Traditional_computing(PID_1, exp);//增量式PID
}

/**
  * 函数作用：  PID2更新函数
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,使用底层传统PID计算函数
  */
int PID2_updata(float exp)
{
  return _PID_Traditional_computing(PID_2, exp);//传统式PID
}

/**
  * 函数作用：  PID3更新函数
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,使用底层传统PID计算函数
  */
int PID3_updata(float exp)
{
  return _PID_Increment_computing(PID_3, exp);//增量式PID
}

/**
  * 函数作用：  闭环控制航向角锁定
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合摄像头用
  * 更新日期：   2026-1-15
  */
int PID_calculate_Yaw_speed(float exp)
{
    return _PID_Increment_computing(PID_2, exp);  
}

/**
  * 函数作用：  闭环控制半径锁定
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合摄像头用
  * 更新日期：   2026-1-15
  */
int PID_calculate_Radius(float exp)
{
    return _PID_Traditional_computing(PID_3, exp);  
}


#if _LINE_USE_
/**
  * 函数作用：  闭环控制巡线
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合摄像头使用
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,调用底层传统PID计算函数
  */
int PID_calculate_Line_Y(float exp)
{
    return _PID_Traditional_computing(PID_Line_Y, exp);  
}


/**
  * 函数作用：  闭环控制巡线
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合摄像头使用
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,调用底层传统PID计算函数
  */
int PID_calculate_Line_X(float exp)
{
  return _PID_Traditional_computing(PID_Line_X, exp);
}

/**
  * 函数作用：  闭环控制巡线
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合摄像头使用
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,调用底层传统PID计算函数
  */
int PID_calculate_DONE_X(float exp)
{
  return _PID_Traditional_computing(PID_DONE_X, exp);
}

/**
  * 函数作用：  闭环控制巡线
  * 参数1：     （float）目标误差
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合摄像头使用
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,调用底层传统PID计算函数
  */
int PID_calculate_DONE_Y(float exp)
{
    return _PID_Traditional_computing(PID_DONE_Y, exp);
}

#endif
#if _T265_USE_

/**
  * 函数作用：  T265闭环速度航向角
  * 参数1：     （float）目标航向角
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合T265使用
  * 更新日期：   2025-11-27
  */
int PID_calculate_Hold_Yaw(float exp)//航向角PID计算
{
    /*误差计算*/
    PID_YAW.err_current = exp - (MY_t265.ryaw / 100);

    /*PID计算*/
    PID_YAW.out = PID_YAW.kp * PID_YAW.err_current
                + PID_YAW.ki * PID_YAW.err_prev
                + PID_YAW.kd * (PID_YAW.err_current - PID_YAW.err_last); 

	PID_YAW.err_last = PID_YAW.err_current;

    /*误差微分*/
    PID_YAW.err_prev += PID_YAW.err_last;

    /*误差限幅*/
    if(ABS(PID_YAW.err_current) < 5) 
        PID_YAW.out = 0;

    /*积分限幅*/
    if (PID_YAW.err_prev > 20.0f)
		PID_YAW.err_prev = 0;
	if (PID_YAW.err_prev < -20.0f)
		PID_YAW.err_prev = 0;

	return limit_pidout(1.5f,PID_YAW.out, PID_YAW.limit_max, PID_YAW.limit_min);
}

/**
  * 函数作用：  T265闭环速度锁定目标_X方向
  * 参数1：     （float）目标X坐标
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合T265使用
  * 更新日期：   2025-11-27
  */
int PID_calculate_t265_x(float exp)
{
    /*误差计算*/
    PID_T265_X.err_current = exp - postion_now_x;

    /*PID计算*/
    PID_T265_X.out = PID_T265_X.kp * PID_T265_X.err_current
                   + PID_T265_X.ki * PID_T265_X.err_prev
                   + PID_T265_X.kd * (PID_T265_X.err_current - PID_T265_X.err_last); 
    /*误差微分*/
	PID_T265_X.err_last = PID_T265_X.err_current;

    /*误差积分*/
    PID_T265_X.err_prev += PID_T265_X.err_last;

    /*误差限幅*/
    if(ABS(PID_T265_X.err_current) < 5)
        PID_T265_X.out = 0;

    /*积分限幅*/
    if (PID_T265_X.err_prev > 20.0f)
		PID_T265_X.err_prev = 0;
	if (PID_T265_X.err_prev < -20.0f)
		PID_T265_X.err_prev = 0;
    
    if(MY_t265.t265_work_sta == 0)
        PID_T265_X.out = 0;
    
    return limit_pidout(1.0f,PID_T265_X.out, PID_T265_X.limit_max, PID_T265_X.limit_min);
}

/**
  * 函数作用：  T265闭环速度锁定目标_Y方向
  * 参数1：     （float）目标Y坐标
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合T265使用
  * 更新日期：   2025-11-27
  */
int PID_calculate_t265_y(float exp)
{
    /*误差计算*/
    PID_T265_Y.err_current = exp - postion_now_y;

    /*PID计算*/
    PID_T265_Y.out = PID_T265_Y.kp * PID_T265_Y.err_current
                              + PID_T265_Y.ki * PID_T265_Y.err_prev
                              + PID_T265_Y.kd * (PID_T265_Y.err_current - PID_T265_Y.err_last);

    /*误差微分*/
	PID_T265_Y.err_last = PID_T265_Y.err_current;

    /*误差积分*/
    PID_T265_Y.err_prev += PID_T265_Y.err_last;

    /*误差限幅*/
    if(ABS(PID_T265_Y.err_current) < 5) 
        PID_T265_Y.out = 0;

    /*积分限幅*/
    if (PID_T265_Y.err_prev > 20.0f)
		PID_T265_Y.err_prev = 0;
	if (PID_T265_Y.err_prev < -20.0f)
		PID_T265_Y.err_prev = 0;

    if(MY_t265.t265_work_sta == 0)
        PID_T265_Y.out = 0;

    return limit_pidout(1.0f,PID_T265_Y.out, PID_T265_Y.limit_max, PID_T265_Y.limit_min);
}

/**
  * 函数作用：  闭环速度锁定目标_X方向
  * 参数1：     （float）目标X坐标
  * 返回值：    （int）PID输出
  * 
  * 备注：      无
  * 更新日期：   2025-11-27
  */
int PID_calculate_attack_x(float exp)
{
    /*误差计算*/
    PID_attack_X.err_current = exp - postion_now_y;

    /*PID计算*/
    PID_attack_X.out = PID_attack_X.kp * PID_attack_X.err_current
                              + PID_attack_X.ki * PID_attack_X.err_prev
                              + PID_attack_X.kd * (PID_attack_X.err_current - PID_attack_X.err_last);

    /*误差微分*/
	PID_attack_X.err_last = PID_attack_X.err_current;

    /*误差积分*/
    PID_attack_X.err_prev += PID_attack_X.err_last;

    /*误差限幅*/
    if(ABS(PID_attack_X.err_current) < 5) 
        PID_attack_X.out = 0;

    /*积分限幅*/
    if (PID_attack_X.err_prev > 20.0f)
		PID_attack_X.err_prev = 0;
	if (PID_attack_X.err_prev < -20.0f)
		PID_attack_X.err_prev = 0;

    return limit_pidout(1.0f,PID_attack_X.out, PID_attack_X.limit_max, PID_attack_X.limit_min);
}

/**
  * 函数作用：  闭环速度锁定目标_Y方向
  * 参数1：     （float）目标Y坐标
  * 返回值：    （int）PID输出
  * 
  * 备注：      无
  * 更新日期：   2025-11-27
  */
int PID_calculate_attack_y(float exp)
{
    /*误差计算*/
    PID_attack_Y.err_current = exp;

    /*PID计算*/
    PID_attack_Y.out = PID_attack_Y.kp * PID_attack_Y.err_current
                     + PID_attack_Y.ki * PID_attack_Y.err_prev
                     + PID_attack_Y.kd * (PID_attack_Y.err_current - PID_attack_Y.err_last);

    /*误差微分*/
	PID_attack_Y.err_last = PID_attack_Y.err_current;

    /*误差积分*/
    PID_attack_Y.err_prev += PID_attack_Y.err_last;

    /*误差限幅*/
    if(ABS(PID_attack_Y.err_current) < 5) 
        PID_attack_Y.out = 0;

    /*积分限幅*/
    if (PID_attack_Y.err_prev > 20.0f)
		PID_attack_Y.err_prev = 0;
	if (PID_attack_Y.err_prev < -20.0f)
		PID_attack_Y.err_prev = 0;

    return limit_pidout(1.0f,PID_attack_Y.out, PID_attack_Y.limit_max, PID_attack_Y.limit_min);
}

/**
  * 函数作用：  闭环速度锁定目标_Z方向
  * 参数1：     （float）目标Z坐标
  * 返回值：    （int）PID输出
  * 
  * 备注：      无
  * 更新日期：   2025-11-27
  */
int PID_calculate_attack_z(float exp)
{
    /*误差计算*/
    PID_attack_Z.err_current = exp;

    /*PID计算*/
    PID_attack_Z.out = PID_attack_Z.kp * PID_attack_Z.err_current
                        + PID_attack_Z.ki * PID_attack_Z.err_prev
                        + PID_attack_Z.kd * (PID_attack_Z.err_current - PID_attack_Z.err_last);

    /*误差微分*/
	PID_attack_Z.err_last = PID_attack_Z.err_current;

    /*误差积分*/
    PID_attack_Z.err_prev += PID_attack_Z.err_last;

    /*误差限幅*/
    if(ABS(PID_attack_Z.err_current) < 5) 
        PID_attack_Z.out = 0;

    /*积分限幅*/
    if (PID_attack_Z.err_prev > 20.0f)
		PID_attack_Z.err_prev = 0;
	if (PID_attack_Z.err_prev < -20.0f)
		PID_attack_Z.err_prev = 0;

    return limit_pidout(1.0f,PID_attack_Z.out, PID_attack_Z.limit_max, PID_attack_Z.limit_min);
}

#endif

/**
  * 函数作用：  N100闭环航向角
  * 参数1：     （float）目标航向角
  * 返回值：    （int）PID输出
  * 
  * 备注：      配合N100使用
  * 更新日期：   2025-11-29
  *             2025-12-7 重构,调用底层传统PID计算函数
  */
int PID_N100_Hold_Yaw(float exp)//航向角PID计算
{
	/*方向最优转化*/
	if((N100.Yaw > exp))	
	{
		if(N100.Yaw - exp < 18000)	//逆时针 负
			PID_YAW.err_current = exp - ((double)N100.Yaw);
		else							//顺时针 正
			PID_YAW.err_current = 36000 - ((double)N100.Yaw) + exp;
	}
	else
	{
		if(exp - N100.Yaw < 18000)	//顺时针	 正
			PID_YAW.err_current = exp - ((double)N100.Yaw);
		else							//逆时针	 负
			PID_YAW.err_current = -(((double)N100.Yaw) + 36000 - exp);
	}
	
  return _PID_Traditional_computing(PID_YAW, PID_YAW.err_current);
}

