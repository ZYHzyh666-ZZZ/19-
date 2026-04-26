#ifndef _PID_H_
#define _PID_H_

#include "ALL_RTOS.h"
#include "LX_FC_State.h"
#include "LX_FC_Fun.h"
#include "Drv_AnoOf.h"
#include "SysConfig.h"
#include "MY_fly_fun.h"
#include "T265.h"

typedef struct
{
	float kp;			//KP
	float ki;			//KI
	float kd;			//KD
	float out;			//输出

	float err_current;	//当前误差
	float err_last;		//上次误差
	float err_prev;		//误差积分

	float limit_max;	//输出限幅上限
	float limit_min;	//输出限幅下限
}_PID_parameter;

extern _PID_parameter	PID_HIGH;
extern _PID_parameter	PID_YAW;

/*--------------------------------------------------------------*/
int limit_pidout(float pid_k, float pidout, float max, float min);
void _PID_Init(_PID_parameter* PID, float P, float I, float D, float out_max, float out_min);
int _PID_Traditional_computing(_PID_parameter PID,float exp);//传统PID计算


/*--------------------------------------------------------------*/

void MY_PID_Init(void);						//PID参数初始化

int PID_calculate_user_high(float exp);		//高度PID

int PID_N100_Hold_Yaw(float exp);//N100航向角PID计算


int PID1_updata(float exp);
int PID2_updata(float exp);
int PID3_updata(float exp);

#endif
