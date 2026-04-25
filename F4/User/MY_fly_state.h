#ifndef __MY_FLY_STATE_H_
#define __MY_FLY_STATE_H_

#include "SysConfig.h"

typedef enum 
{
	OFF = 0,
	ON = 1,
}bool;

typedef enum 
{
    _LX = 0,
    _OF,
    _Sbus,
    _N100,
    _Camera,
    error_sensor
}External_dictator;


typedef struct
{
/*			姿态角（度）		*/
	float pitch;
	float roll;
	float yaw;

/*		机体坐标系速度（cm/s）	*/
	float Velocity_x;
	float Velocity_y;
	float Velocity_z;

/*        机体坐标系坐标（cm）  */
    float location_x;
    float location_y;
    float location_z;

	u16 pwm_1;
	u16 pwm_2;
	u16 pwm_3;
	u16 pwm_4;

}_MY_body_def;

typedef struct
{
/*			外设状态（1正常）	*/
	bool LX_state;
	bool OF_state;
	bool Sbus_state;
	bool N100_state;
	bool Camera_state;

    u8 error_state;

/*		工作状态	*/
	bool fly_state;		//1  已起飞
	bool land_state;	//1  已降落
	float work_time;	//秒 单位
	bool lock_flag;		//1  锁桨
	bool work_mod;		//X  工作模式

}_MY_work_def;

typedef struct
{
/*			坐标系			*/
	float x;
	float y;
	float z;

/*			坐标系速度		*/
	float Velocity_X;
	float Velocity_Y;
	float Velocity_Z;

/*		飞机的航向角差		*/
	float yaw_difference;

}_MY_C_system_def;

typedef struct 
{
	float Velocity_X;
	float Velocity_Y;
	float Velocity_Z;

	float pitch;
	float roll;
	float yaw;

	s16 location_x;
	s16 location_y;
	s16 location_z;

}_MY_target_def;


typedef struct
{
	_MY_body_def body;
	_MY_work_def work;
	_MY_C_system_def C_system;
	_MY_target_def target;

}_MY_fly_def;

extern _MY_fly_def MY_fly;


/*-----------------函数声明----------------------*/

void Create_System(void);										//系统初始化
void MY_work_time_update(float time_interval);					//系统时间更新函数
void MY_System_transformation(float time_interval);				//机体坐标系到基坐标系转换

External_dictator MY_external_sensor_state_update(External_dictator Extern, bool State);		//更新外部传感器状态
void MY_body_state_update(float pitch, float roll, float yaw, float Vx, float Vy, float Vz);	//更新机体姿态和速度信息
void MY_body_pwm_update(u16 pwm1, u16 pwm2, u16 pwm3, u16 pwm4);	//更新机体PWM输出值




#endif
