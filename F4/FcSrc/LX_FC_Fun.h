#ifndef __LX_FC_FUN_H
#define __LX_FC_FUN_H

//==引用
#include "SysConfig.h"

//==定义/声明

//==数据声明

//==函数声明
//static

//public
u8 FC_Unlock(void);
u8 FC_Lock(void);
u8 LX_Change_Mode(u8 new_mode);
u8 OneKey_Takeoff(u16 height_cm);
u8 OneKey_Return_Home(void);
u8 Horizontal_Calibrate(void);
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360);
u8 Mag_Calibrate(void);
u8 ACC_Calibrate(void);
u8 GYR_Calibrate(void);


void Program_Ctrl_User_Set_HXYcmps(s16 hx_vel_cmps,s16 hy_vel_cmps);//航向水平坐标系下速度设定（实时控制）
void Program_Ctrl_User_Set_Zcmps(s16 z_vel_cmps);//上升下降速度设定（实时控制）
void Program_Ctrl_User_Set_YAWdps(s16 yaw_pal_dps);//航向速度设定（实时控制）
void Rt_tar_Stop(void);//实时控制速度归零

#endif
