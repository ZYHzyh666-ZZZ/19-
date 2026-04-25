#ifndef __LX_FC_STATE_H
#define __LX_FC_STATE_H

//==引用
#include "SysConfig.h"

//==定义/声明
typedef struct
{
	u8 pre_locking;//预备上锁
	u8 stick_mit_pos;//摇杆中位

} _sticks_fun_st;

typedef struct
{
	u8 CID;
	u8 CMD_0;
	u8 CMD_1;
} _cmd_fun_st;
//飞控状态
typedef struct
{
	//模式
	u8 fc_mode_cmd;//飞行模式命令
	u8 fc_mode_sta;//当前飞行模式

	//解锁上锁，0上锁
	u8 unlock_cmd;//解锁上锁命令
	u8 unlock_sta;//当前解锁上锁状态

	//指令功能
	_cmd_fun_st cmd_fun;

	//state
	u8 imu_ready;//imu准备好
	u8 take_off;//起飞标志位
	u8 in_air;//飞行标志位
	u8 landing;//降落标志位

} _fc_state_st;

//==数据声明
extern _fc_state_st fc_sta;
extern _sticks_fun_st sti_fun;
//==函数声明
//static

//public
void LX_FC_State_Task(float dT_s);

#endif
