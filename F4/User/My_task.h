#include "SysConfig.h"
#include "Ano_Scheduler.h"
#include "ALL_RTOS.h"

/*			任务优先级			*/
#define START_TASK_PRIO		        1
#define ANO_TASK_PRIO		        2

#define Basic_TASK_PRIO		        5
#define User_TASK_PRIO		        15
#define Systeam_TASK_PRIO	        8

#define USART_TASK_PRIO		        17
#define BAT_TASK_PRIO	    	    9
#define ANO_Exchange_TASK_PRIO	    15
#define FC_State_TASK_PRIO		    13
#define RC_TASK_PRIO			    10
#define ESC_Out_TASK_PRIO		    16

/*          任务堆大小          */
#define START_STK_SIZE 		        512
#define ANO_STK_SIZE 	        	512

#define Basic_STK_SIZE 		        512
#define User_STK_SIZE 		        256
#define Systeam_STK_SIZE 	        256

#define USART_STK_SIZE		        256
#define BAT_STK_SIZE		        128
#define ANO_Exchange_STK_SIZE		128
#define FC_State_STK_SIZE		    128
#define RC_STK_SIZE			        128
#define ESC_Out_STK_SIZE		    64

/*			任务曲柄			*/
extern TaskHandle_t     StartTask_Handler;          //开始任务（创建所有任务）
extern TaskHandle_t     ANO_Handler;				//ANO外设创建回调

extern TaskHandle_t     Basic_Handler;              //基础外设
extern TaskHandle_t     User_Handler;               //主任务程序
extern TaskHandle_t     Systeam_Handler;            //坐标系转化更新

extern TaskHandle_t		USART_Handler;				//串口数据检测
extern TaskHandle_t		BAT_Handler;				//电池电压检测
extern TaskHandle_t		ANO_Exchange_Handler;		//外部传感器数据处理任务
extern TaskHandle_t		FC_State_Handler;			//飞控状态更新任务
extern TaskHandle_t		RC_Handler;				    //遥控数据处理任务
extern TaskHandle_t		ESC_Out_Handler;			//电调输出任务

extern TimerHandle_t 	user_200Hz_Handle;			//周期定时器句柄
/*		    任务回调函数		*/
void start_task(void *pvParameters);
void ANO_task(void *pvParameters);					//ANO外设创建回调

void Basic_task(void *pvParameters);                //基础外设
void User_task(void *pvParameters);                 //主任务程序
void Systeam_task(void *pvParameters);              //坐标系转化更新

void USART_task(void *pvParameters);              	//串口数据检测
void BAT_task(void *pvParameters);                  //电池电压检测
void ANO_Exchange_task(void *pvParameters);         //外部传感器数据处理任务
void FC_State_task(void *pvParameters);            //飞控状态更新任务
void RC_task(void *pvParameters);					//遥控数据处理任务
void ESC_Out_task(void *pvParameters);				//电调输出任务

void user_200Hz(TimerHandle_t xTimer); 				//周期定时器回调函数
/*-------------------------------*/

extern s16 postion_target_x, velocity_target_x;
extern s16 postion_target_y, velocity_target_y;
extern s16 postion_target_z;
extern double Work_time;

