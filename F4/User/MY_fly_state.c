#include "MY_fly_state.h"
#include "User_Math.h"
#include "ANO_DT_LX.h"

#define PI 3.1415926535f

_MY_fly_def MY_fly;

/*-----------------结构体----------------------*/
void Init_body_state(void)
{
    MY_fly.body.pitch = 0;
    MY_fly.body.roll = 0;
    MY_fly.body.yaw = 0;

    MY_fly.body.Velocity_x = 0;
    MY_fly.body.Velocity_y = 0;
    MY_fly.body.Velocity_z = 0;

    MY_fly.body.location_x = 0;
    MY_fly.body.location_y = 0;
    MY_fly.body.location_z = 0;

    MY_fly.body.pwm_1 = 0;
    MY_fly.body.pwm_2 = 0;
    MY_fly.body.pwm_3 = 0;
    MY_fly.body.pwm_4 = 0;
}

void Init_work_state(void)
{
    MY_fly.work.LX_state = OFF;
    MY_fly.work.OF_state = OFF;
    MY_fly.work.Sbus_state = OFF;
    MY_fly.work.N100_state = OFF;
    MY_fly.work.Camera_state = OFF;
    MY_fly.work.error_state = OFF;

    MY_fly.work.fly_state = OFF;
    MY_fly.work.land_state = OFF;
    MY_fly.work.work_time = 0;
    MY_fly.work.lock_flag = OFF;
    MY_fly.work.work_mod = OFF;
}

void Init_C_system_state(void)
{
    MY_fly.C_system.x = 0;
    MY_fly.C_system.y = 0;
    MY_fly.C_system.z = 0;

    MY_fly.C_system.Velocity_X = 0;
    MY_fly.C_system.Velocity_Y = 0;
    MY_fly.C_system.Velocity_Z = 0;

    MY_fly.C_system.yaw_difference = 0;
}

void Init_target_state(void)
{
    MY_fly.target.Velocity_X = 0;
    MY_fly.target.Velocity_Y = 0;
    MY_fly.target.Velocity_Z = 0;

    MY_fly.target.location_x = 0;
    MY_fly.target.location_y = 0;
    MY_fly.target.location_z = 0;

    MY_fly.target.pitch = 0;
    MY_fly.target.roll = 0;
    MY_fly.target.yaw = 0;
}


/*----------------------------------------------------------------*/
/*----------------------------应用层-----------------------------*/
/**
  * 函数作用：  系统初始化
  * 参数：      无
  * 返回值：    无
  * 
  * 备注：      放在最开始调用
  * 更新日期：   2026-01-14
  */

void Create_System(void)
{
    Init_body_state();          //机体坐标系
    Init_work_state();          //外部传感器工作状态
    Init_C_system_state();      //基坐标系
    Init_target_state();        //目标状态
}

/**
  * 函数作用：  更新系统时间
  * 参数1：     （float）时间间隔，单位秒
  * 返回值：    无
  * 
  * 备注：      放在稳定定时器中断中调用，单位秒
  * 更新日期：   2026-01-14
  */
void MY_work_time_update(float time_interval)
{
    //工作时间更新
    MY_fly.work.work_time += time_interval;
}

/**
  * 函数作用：  更新外部传感器状态
  * 参数1：     （External_dictator）传感器名称
  * 参数2：     （bool）传感器状态
  * 返回值：    （External_dictator）异常传感器名称，无异常返回error_sensor
  * 
  * 备注：      传感器名称为error_sensor时，表示查询当前异常传感器
  * 更新日期：   2026-01-14
  */
External_dictator MY_external_sensor_state_update(External_dictator Extern, bool State)
{
    switch(Extern)
    {
        case _LX:
            MY_fly.work.LX_state = State;
            break;
        case _OF:
            MY_fly.work.OF_state = State;
            break;
        case _Sbus:
            MY_fly.work.Sbus_state = State;
            break;
        case _N100:
            MY_fly.work.N100_state = State;
            break;
        case _Camera:
            MY_fly.work.Camera_state = State;
            break;
    }

    MY_fly.work.error_state = ~(MY_fly.work.LX_state && MY_fly.work.OF_state && MY_fly.work.Sbus_state && MY_fly.work.N100_state && MY_fly.work.Camera_state);
    if(MY_fly.work.error_state == 0)
        return (External_dictator)error_sensor;   //系统正常
    else
    {
        for(External_dictator i = _LX; i <= _Camera; i++)
        {
            if(*(&MY_fly.work.LX_state + i) == OFF)
                return (External_dictator)i;   //返回哪个传感器异常
        }
    }
    return (External_dictator)error_sensor;   //系统正常
}

/**
  * 函数作用：  更新机体姿态和速度信息
  * 参数1：     （float）俯仰角
  * 参数2：     （float）横滚角
  * 参数3：     （float）偏航角
  * 参数4：     （float）机体X轴速度 cm/s
  * 参数5：     （float）机体Y轴速度 cm/s
  * 参数6：     （float）机体Z轴速度 cm/s
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2026-01-14
  */
void MY_body_state_update(float pitch, float roll, float yaw, float Vx, float Vy, float Vz)
{
    MY_fly.body.pitch = pitch;
    MY_fly.body.roll = roll;
    MY_fly.body.yaw = yaw;

    MY_fly.body.Velocity_x = Vx;
    MY_fly.body.Velocity_y = Vy;
    MY_fly.body.Velocity_z = Vz;
}

/**
  * 函数作用：  更新机体PWM输出值
  * 参数1：     （s16）PWM1值
  * 参数2：     （s16）PWM2值
  * 参数3：     （s16）PWM3值
  * 参数4：     （s16）PWM4值
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2026-01-14
  */
void MY_body_pwm_update(u16 pwm1, u16 pwm2, u16 pwm3, u16 pwm4)
{
    MY_fly.body.pwm_1 = pwm1;
    MY_fly.body.pwm_2 = pwm2;
    MY_fly.body.pwm_3 = pwm3;
    MY_fly.body.pwm_4 = pwm4;
}

/**
  * 函数作用：  机体坐标系向基坐标系转换
  * 参数1：     （float）时间间隔，单位秒
  * 返回值：    无
  * 
  * 备注：      暂时只考虑偏航角，不考虑俯仰和横滚
  *             机体偏航角90度对应基坐标系0度
  *             坐标XY积分获得,Z光流获得，
  *             速度来源机体光流，偏航角来源n100
  * 更新日期：   2026-01-14
  */
void MY_System_transformation(float time_interval)
{
    double yaw_rad;
    double cos_yaw;
    double sin_yaw;

    yaw_rad = change_angle_range(MY_fly.body.yaw - 90.0f) * PI / 180.0f;
    cos_yaw = cos_f(yaw_rad);
    sin_yaw = sin_f(yaw_rad);

    MY_fly.C_system.Velocity_X = MY_fly.body.Velocity_x * cos_yaw + MY_fly.body.Velocity_y * sin_yaw;
    MY_fly.C_system.Velocity_Y = -MY_fly.body.Velocity_x * sin_yaw + MY_fly.body.Velocity_y * cos_yaw;
    MY_fly.C_system.Velocity_Z = MY_fly.body.Velocity_z;  //不可靠

    MY_fly.C_system.x += MY_fly.C_system.Velocity_X * time_interval;
    MY_fly.C_system.y += MY_fly.C_system.Velocity_Y * time_interval;
    MY_fly.C_system.z = MY_fly.body.location_z;             //直接测距

    MY_fly.C_system.yaw_difference = change_angle_range(MY_fly.body.yaw - 90.0f);

    // ANO_DT_Send_MY_DATA(0xF1,2,(s16)(MY_fly.C_system.x),(s16)(MY_fly.C_system.y));   
}
