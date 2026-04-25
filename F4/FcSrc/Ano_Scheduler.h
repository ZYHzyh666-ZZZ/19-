#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "SysConfig.h"
typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;//频率
uint16_t interval_ticks;//延时时间
uint32_t last_run;//上次执行的时间
}sched_task_t;

void Scheduler_Setup(void);
void Scheduler_Run(void);

#endif

