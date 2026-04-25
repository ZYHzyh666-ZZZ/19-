#include "SysConfig.h"
#include "Ano_Scheduler.h"
#include "ALL_RTOS.h"

/*			任务优先级			*/
#define START_TASK_PRIO		1
#define ANO_TASK_PRIO		2

#define Basic_TASK_PRIO		5
#define User_TASK_PRIO		5
#define Systeam_TASK_PRIO	10
#define Flight_TASK_PRIO	10

#define USART_TASK_PRIO		15


/*          任务堆大小          */
#define START_STK_SIZE 		64  
#define ANO_STK_SIZE 		64  

#define Basic_STK_SIZE 		128  
#define User_STK_SIZE 		128  
#define Systeam_STK_SIZE 	128  

#define USART_STK_SIZE		128

/*			任务曲柄			*/
extern TaskHandle_t     StartTask_Handler;
extern TaskHandle_t     ANO_Handler;				//ANO外设创建回调

extern TaskHandle_t     Basic_Handler;              //基础外设
extern TaskHandle_t     User_Handler;               //主任务程序
extern TaskHandle_t     Systeam_Handler;            //坐标系转化更新

extern TaskHandle_t		USART_Handler;				//串口数据检测

extern TimerHandle_t 	user_200Hz_Handle;			//周期定时器句柄
/*		    任务回调函数		*/
void start_task(void *pvParameters);
void ANO_task(void *pvParameters);					//ANO外设创建回调

void Basic_task(void *pvParameters);                //基础外设
void User_task(void *pvParameters);                 //主任务程序
void Systeam_task(void *pvParameters);              //坐标系转化更新

void USART_task(void *pvParameters);              	//串口数据检测

void user_200Hz(TimerHandle_t xTimer); 				//周期定时器回调函数
/*-------------------------------*/

extern s16 postion_target_x ;
extern s16 postion_target_y;
extern s16 postion_target_z;
extern double Work_time;

