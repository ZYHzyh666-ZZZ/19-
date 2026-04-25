#ifndef _MY_FLY_FUN_H_
#define _MY_FLY_FUN_H_

#include "SysConfig.h"
#include "MY_PID.h"
#include "math.h"
#include "T265.h"
#include "stdio.h"

extern s16 x_speed;
extern s16 y_speed;
extern s16 z_speed;
extern u8 begin_land;//开始降落标志

extern s16 postion_set_x[20];
extern s16 postion_set_y[20];


void My_onekey_lock(void);//一键锁桨
void My_onekey_unlock(void);//一键解锁

void Move_stop(void);                                       //停止移动
void Set_High(float high_cm);								//上升，参数上升距离，正上负下
void mode_Hold_Yaw(s16 exp_yaw);							//航向角控制

void Circular_rotation(s16 r, s16 v);                       //圆形旋转，参数为半径cm，速度cm/s
void Reach_Circular(s16 Radius, s16 Speed, float time, float angle);     //到达指定圆周，参数为半径cm，速度cm/s，时间s

void Circular_Radius(s16 r_error, int v_y);                      //距离锁定，参数为距离差像素
void Circular_center(s16 r_error);                       //角度锁定，参数为角度差像素
#if 0
void reach_line(s16 line_exp_x, s16 line_exp_y, u8 x_stste, u8 y_state);    //巡线，参数为目标X轴误差，目标Y轴误差，X轴PID使能状态位，Y轴PID使能状态位
u8 reach_land(s16 line_exp_x, s16 line_exp_y);			//定点
#endif

#if _T265_USE_

u8 reach_postion(s16 postion_exp_x , s16 postion_exp_y);	//到达指定区域，参数为期望位置坐标
u8 reach_attack_postion(s16 attack_exp_x, s16 attack_exp_z);      //到达指定打击区域
#endif

#endif
