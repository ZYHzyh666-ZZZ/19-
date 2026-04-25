#ifndef __T265_H
#define __T265_H

#include "ANO_LX.h"
#include "LX_FC_State.h"
#include "Drv_AnoOf.h"

#define recieve_ok 1


typedef struct
{
    /*三轴坐标原始数据（毫米）*/
    s16   x_coordinate;    //当x为0.1m的时候    	x_coordinate等于100
    s16   y_coordinate;    //当x为3m的时候   	x_coordinate等于3000
    s16   z_coordinate;

    /*三轴速度*/
    s16   vx_coordinate;
    s16   vy_coordinate;
    s16   vz_coordinate;

    /*姿态角*/
    u16   rpicth;
    u16   rroll;
    u16   ryaw;

    u16 of_t265_update_cnt;  //T265速度等数据更新计数。
	u16 of_t265_update_cnt_last;
    u8 alt_t265_update_cnt; //高度数据更新计数。
    
    u8 t265_link_sta; //连接状态：0，未连接。1，已连接。
	u8 t265_work_sta; //工作状态：0，异常。1，正常
}T265;

/*当前位置（毫米）*/
extern s16 postion_now_x;
extern s16 postion_now_y;
extern s16 postion_now_z;

extern u8 T265_start;
extern u8 T265_Start_Flag,T265_Direction;
extern T265 MY_t265;

extern u16 data_sum_last;
extern u16 data_sum;
extern u8 T265_data[30];                                   


void T265_check(void);
void T265_Data(void);
void T265_Receive_Data(uint8_t data);
void speed_calculate_value(void);


#endif
