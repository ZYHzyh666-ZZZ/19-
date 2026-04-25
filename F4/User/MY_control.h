#ifndef _MY_CONTROL_H_
#define _MY_CONTROL_H_

#include "ALL_RTOS.h"
#include "LX_FC_State.h"
#include "LX_FC_Fun.h"
#include "Drv_AnoOf.h"

typedef struct
{
	void(*task_mission)(void);	//任务函数名
	float time;               	//任务执行时间
}My_mission_task;				//分任务结构体

typedef struct
{
	u8 finish;					//任务完成标志位
	u8 step;					//任务进度
	u8 task_num;				//任务个数
	float mission_times; 		//任务执行时间
	My_mission_task* mission;	//分任务
}My_task_state;					//任务状态结构体

extern u8 start_task1_flag;		//任务开始标志
extern My_task_state task_1;	//任务结构体1

extern u8 start_task2_flag;		//任务开始标志
extern My_task_state task_2;	//任务结构体2
/*-----------------------------------------------------*/

#define _time_ok(task)	(task.mission_times > task.mission[task.step].time)

void create_task_1(void);		//创建任务
void create_task_2(void);		//创建任务

void task_run(u16 ms);			//任务运行

void mission_1_1(void);
void mission_1_2(void);
void mission_1_3(void);
void mission_1_4(void);
void mission_1_5(void);
void mission_1_6(void);
void mission_1_7(void);
void mission_1_8(void);
void mission_1_9(void);
void mission_1_10(void);
void mission_1_11(void);
void mission_1_12(void);
void mission_1_13(void);
void mission_1_14(void);
void mission_1_15(void);

void mission_2_1(void);
void mission_2_2(void);
void mission_2_3(void);
void mission_2_4(void);
void mission_2_5(void);
void mission_2_6(void);
void mission_2_7(void);
void mission_2_8(void);

#endif
