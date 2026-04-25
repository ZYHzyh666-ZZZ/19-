#include "MY_fly_fun.h"
#include "Uart_tiva.h"
#include "MY_fly_state.h"

#define ABS(x) ((x)>0?(x):-(x))
#define PI 3.1415926

/*速度PID存放变量*/
s16 x_speed;
s16 y_speed;
s16 z_speed;

/*降落表示位*/
u8 begin_land = 0;


s16 postion_set_x[20]={0};
s16 postion_set_y[20]={0};

/**
  * 函数作用：  一键锁桨
  * 参数：      无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025
  */
void My_onekey_lock(void)
{
	if(fc_sta.unlock_sta == 1)//上锁一次
		FC_Lock();
	else
		return;
}


/**
  * 函数作用：  一键解锁
  * 参数：      无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025
  */
void My_onekey_unlock(void)
{
	if(fc_sta.unlock_sta == 0)//解锁一次
		FC_Unlock();
	else
		return;
}


/**
  * 函数作用：  停止移动
  * 参数：      无
  * 返回值：    无
  * 
  * 备注：      停止XY以及YAW的移动
  * 更新日期：   2026-1-15
  */
void Move_stop(void)
{
	Program_Ctrl_User_Set_YAWdps(0);
    Program_Ctrl_User_Set_HXYcmps(0,0);
}

/**
  * 函数作用：  上升函数
  * 参数1：     （float）目标高度
  * 返回值：    无
  * 
  * 备注：      配合匿名光流使用
  * 更新日期：   2025-11-27
  */
void Set_High(float high_cm)
{
	Program_Ctrl_User_Set_Zcmps(PID_calculate_user_high(high_cm));
}

/**
  * 函数作用：  航向角控制
  * 参数1：     （s16）目标航向角*100
  * 返回值：    无
  * 
  * 备注：      配合N100的航向角使用
  * 更新日期：   2025-11-29
  */
void mode_Hold_Yaw(s16 exp_yaw)
{
    float Target_Yaw;
    /*计算航向角PID*/
    Target_Yaw = PID_N100_Hold_Yaw(exp_yaw);
    /*速度上传*/
    Program_Ctrl_User_Set_YAWdps(Target_Yaw);
//	ANO_DT_Send_MY_DATA_3();
}

/**
  * 函数作用：  圆形旋转
  * 参数1：     (s16)半径cm
  * 参数2：     (s16)速度cm/s
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2026-1-13
  */
void Circular_rotation(s16 r, s16 v)
{
    s16 Target_Yaw_speed;
    /*计算航向角速度*/
    Target_Yaw_speed = (v*360)/(2*PI*r)*10;  //cm/s转为deg/s

    Target_Yaw_speed = (Target_Yaw_speed-5)/10; //四舍五入
    /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(0,v);
    Program_Ctrl_User_Set_YAWdps(Target_Yaw_speed);


//	ANO_DT_Send_MY_DATA_3();
}


/**
  * 函数作用：  闭环旋转
  * 参数1：     （s16）目标半径
  * 参数2：     （s16）目标速度
  * 参数3：     （float）时间s
  * 参数4：     （float）角度
  * 返回值：     无
  * 
  * 备注：      配合N100的航向角使用,需要配合时间,通过时间计算出角度值
  *             速度直接传出，角度进入PID计算
  * 更新日期：   2026-1-13
  */
void Reach_Circular(s16 Radius, s16 Speed, float time, float angle)
{
    float angle_100 = 0;
    int Target_Yaw_speed;

    angle_100 = (Speed*time*360)/(2*PI*Radius) * 100; //计算出旋转角度
    angle_100 = ((int)(angle*100 + angle_100 + 18000))%36000 - 18000; //转换为N100航向角格式

    Target_Yaw_speed = PID_calculate_Circular(angle_100);

    ANO_DT_Send_MY_DATA(0xF1,2,(int)angle_100,(int)(angle_100 - N100.Yaw));
    // /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(0,Speed);
	Program_Ctrl_User_Set_YAWdps(Target_Yaw_speed);

}

/**
  * 函数作用：  距离锁定
  * 参数1：     (s16)距离差cm
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2026-1-13
  */
void Circular_Radius(s16 r_error, int v_y)
{
    int V_x;
    V_x = PID_calculate_Radius(r_error);
    /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(V_x,v_y);
    // ANO_DT_Send_MY_DATA(0xF1,1,V_x);
//	ANO_DT_Send_MY_DATA_3();
}

/**
  * 函数作用：  自转角度锁定
  * 参数1：     (s16)角度差像素
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2026-1-13
  *             2026-1-17
  */
void Circular_center(s16 r_error)
{
    int angle_speed;

    angle_speed = MY_fly.body.Velocity_y * 360 / 70 / 2 / PI;

    angle_speed += PID_calculate_Yaw_speed(r_error);
    /*速度上传*/
    Program_Ctrl_User_Set_YAWdps(angle_speed);
    // ANO_DT_Send_MY_DATA(0xF1,1,V_yaw);
//	ANO_DT_Send_MY_DATA_3();
}

#if 0

/**
  * 函数作用：  闭环前进巡线
  * 参数1：     （float）目标X轴误差/速度
  * 参数2：     （float）目标Y轴误差/速度
  * 参数3：     (u8)X轴PID使能状态位
  * 参数4：     (u8)Y轴PID使能状态位
  * 返回值：     (u8)到达标志位
  * 
  * 备注：      必须配合视觉作闭环控制
  * 更新日期：   2025-12-6
  *             2025-12-7 重构，增加PID使能状态位
  */
void reach_line(s16 line_exp_x, s16 line_exp_y, u8 x_stste, u8 y_state)
{
    /*速度PID计算*/
    if(x_stste == 1)
        x_speed = PID_calculate_Line_X((float)(line_exp_x));
    else
        x_speed = (float)line_exp_x;
    
    if(y_state == 1)
        y_speed = PID_calculate_Line_Y((float)(line_exp_y));
    else
        y_speed = (float)line_exp_y;
//	ANO_DT_Send_MY_DATA_1(x_speed,y_speed);
    /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(x_speed,y_speed);

}

/**
  * 函数作用：  闭环锁定圆
  * 参数1：     （float）目标X误差
  * 参数2：     （float）目标Y误差
  * 返回值：     (u8)到达标志位
  * 
  * 备注：      必须配合视觉作闭环控制
  * 更新日期：   2025-12-6
  */
u8 reach_land(s16 line_exp_x, s16 line_exp_y)
{
    /*速度PID计算*/
    x_speed = PID_calculate_DONE_X((float)(line_exp_x));
    y_speed = PID_calculate_DONE_Y((float)(line_exp_y));
//	ANO_DT_Send_MY_DATA_1(x_speed,y_speed);
    /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(x_speed,y_speed);
	
	if(ABS(line_exp_x) < 40 && ABS(line_exp_y) < 40)
		return 1;
	else
		return 0;
}

#endif

#if _T265_USE_

/**
  * 函数作用：  到达指定位置
  * 参数1：     (s16)目标X坐标
  * 参数2：     (s16)目标Y坐标
  * 返回值：    (u8)到达标志位
  * 
  * 备注：      配合T265使用，降落时PID参数有所调整且死区变小
  * 更新日期：   2025-11-27
  */
u8 reach_postion(s16 postion_exp_x , s16 postion_exp_y)
{
	static u8 get = 0;

    /*死区标志位*/
	if(begin_land == 1)
	{
		PID_T265_X.kp = 1;
		PID_T265_Y.kp = 1;
        if((ABS(postion_now_x-postion_exp_x) <= 8) && (ABS(postion_now_y-postion_exp_y) <= 8))//位置坐标差小于8cm
            get++;
        else
            get = 0;
	}
    else
    {
        if((ABS(postion_now_x-postion_exp_x) <= 15) && (ABS(postion_now_y-postion_exp_y) <= 15))//位置坐标差小于15cm
            get++;
        else 
            get = 0;
    }

    /*速度PID计算*/
    x_speed=PID_calculate_t265_x((float)(postion_exp_x));
    y_speed=PID_calculate_t265_y((float)(postion_exp_y));
    
    /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(x_speed,y_speed);
    
    if(get>=3)//连续三次坐标差距小于阈值
    {
        get=0;
        return 1;
    }
    else
        return 0;
}


/**
  * 函数作用：  闭环锁定目标立牌
  * 参数1：     （float）目标Y坐标
  * 参数2：     （float）目标Z坐标
  * 返回值：     (u8)到达标志位
  * 
  * 备注：      必须配合视觉作闭环控制
  * 更新日期：   2025-11-27
  */
u8 reach_attack_postion(s16 attack_exp_y, s16 attack_exp_z)
{
	static u8 get = 0;

    /*死区标志位*/
    if((ABS(attack_exp_y)<=13)&&(ABS(attack_exp_z)<=13))//位置坐标差小于15cm
        get++;
    else 
        get = 0;

    /*速度PID计算*/
    y_speed=PID_calculate_attack_y((float)(attack_exp_y));
    z_speed=PID_calculate_attack_z((float)(attack_exp_z));
    /*速度上传*/
    Program_Ctrl_User_Set_HXYcmps(0,y_speed);
    Program_Ctrl_User_Set_Zcmps(z_speed);

    if(get>=3)//连续三次坐标差距小于阈值
    {
        get=0;
        return 1;
    }
    else
        return 0;

}

#endif

