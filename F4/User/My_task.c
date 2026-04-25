#include "ALL_RTOS.h"
#include "My_task.h"
#include "Drv_led.h"
#include "Drv_key.h"
#include "MY_fly_fun.h"
#include "Drv_Uart.h"
#include "T265.h"
#include "Drv_beep.h"
#include "MY_Gstation.h"
#include "MY_Camera.h"
#include "Uart_tiva.h" 
#include "ANO_DT_LX.h"
#include "MY_fly_state.h"
#include "MY_control.h"
#include "Drv_uart.h"


/*      任何曲柄和任务函数      */
TaskHandle_t StartTask_Handler;             //开始任务（创建所有任务）
void start_task(void *pvParameters);


TaskHandle_t Task1Task_Handler;
void task1_task(void *pvParameters);


TaskHandle_t Task2Task_Handler;
void task2_task(void *pvParameters);


TaskHandle_t Basic_Handler;             	//基础外设
void Basic_task(void *pvParameters);


TaskHandle_t User_Handler;            		//主任务程序
void User_task(void *pvParameters);


TaskHandle_t Systeam_Handler;             	//坐标系转化更新
void Systeam_task(void *pvParameters);

//user_control任务周期 单位ms
#define mission_period		5
TimerHandle_t 	user_200Hz_Handle;			
void user_200Hz(TimerHandle_t xTimer); 	


TaskHandle_t ANO_Handler;             		//ANO外设创建
void ANO_task(void *pvParameters);

TaskHandle_t USART_Handler;             	//串口数据更新
void USART_task(void *pvParameters);

/*---------------------------------*/
u32 times;
double Work_time = 0;

u16 User_times_count; //
s16 postion_target_x; 
s16 postion_target_y;
s16 postion_target_z;
/*---------------------------------*/

//空闲任务钩子函数
void vApplicationIdleHook(void)
{
	
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	  //创建软件周期定时器
    user_200Hz_Handle = xTimerCreate( (const char*		)"user_200Hz",
                                    (TickType_t			)mission_period,
                                    (UBaseType_t		)pdTRUE,
                                    (void*				)1,//ID号
							        (TimerCallbackFunction_t)user_200Hz); //周期定时器，周期5ms(5个时钟节拍)，周期
    //创建ANO相关外设任务
    xTaskCreate((TaskFunction_t )ANO_task,             
                (const char*    )"ANO_task",           
                (uint16_t       )ANO_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )ANO_TASK_PRIO,        
                (TaskHandle_t*  )&ANO_Handler);   
	//创建基础外设任务
	xTaskCreate((TaskFunction_t )Basic_task,     
				(const char*    )"Basic_task",   
				(uint16_t       )Basic_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )Basic_TASK_PRIO,
				(TaskHandle_t*  )&Basic_Handler); 
	//创建主任务调用程序
	xTaskCreate((TaskFunction_t )User_task,     
				(const char*    )"User_task",   
				(uint16_t       )User_STK_SIZE,
				(void*          )NULL,
				(UBaseType_t    )User_TASK_PRIO,
				(TaskHandle_t*  )&User_Handler); 

	xTimerStart(user_200Hz_Handle,0);	//开启周期定时器
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}
/**
  * 函数作用：  ANO相关函数创建回调函数
  * 参数1：     参数句柄
  * 返回值：    无
  * 
  * 备注：      基于RTOS的外设调用函数，优先级 2
  * 更新日期：   2026-1-22
  */
void ANO_task(void *pvParameters)//
{
	taskENTER_CRITICAL();           //进入临界区
	//创建USART任务
    xTaskCreate((TaskFunction_t )USART_task,             
                (const char*    )"USART_task",           
                (uint16_t       )USART_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USART_TASK_PRIO,        
                (TaskHandle_t*  )&USART_Handler); 
			

    vTaskDelete(ANO_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

/**
  * 函数作用：  USART更新回调函数
  * 参数1：     参数句柄
  * 返回值：    无
  * 
  * 备注：      解析串口数据任务，优先级 15
  * 更新日期：   2026-1-22
  */
void USART_task(void *pvParameters)//
{	
	while(1)
    {		
		drvU1DataCheck();
		drvU2DataCheck();
		// drvU3DataCheck();//sbus
		drvU4DataCheck();
		drvU5DataCheck();
		drvU6DataCheck();
        vTaskDelay(1);
    }
}


/**
  * 函数作用：  基础外设调用回调函数
  * 参数1：     参数句柄
  * 返回值：    无
  * 
  * 备注：      基于RTOS的外设调用函数，优先级 1
  * 更新日期：   2026-1-22
  */
void Basic_task(void *pvParameters)
{
	while(1)
    {		
        KEY_updata();							//MY_key  
		MY_beep_20ms();			                //MY_beep 
		MY_led_light(5);                        //MY_LED  
		
		Tiva_postion_print((s16)(MY_fly.C_system.x),(s16)(MY_fly.C_system.y),(s16)ano_of.of_alt_cm); 
        // ANO_DT_Send_MY_DATA(0xF1,2,(s16)(MY_fly.C_system.Velocity_X),(s16)(MY_fly.C_system.Velocity_Y));
        ANO_DT_Send_MY_DATA(0xF5,4,(s16)(MY_fly.work.work_time),(s16)(MY_fly.C_system.x),(s16)(MY_fly.C_system.y),
                            (s16)(MY_fly.C_system.z));
        vTaskDelay(5);
    }
}


/**
  * 函数作用：  坐标系转化回调函数
  * 参数1：     参数句柄
  * 返回值：    无
  * 
  * 备注：      基于RTOS的外设调用函数，优先级 10
  * 更新日期：   2026-1-22
  */
void Systeam_task(void *pvParameters)
{
    while(1)
    {

    }
}


/**
  * 函数作用：  主任务调用回调函数
  * 参数1：     参数句柄
  * 返回值：    无
  * 
  * 备注：      基于RTOS的主任务调用，优先级 20
  * 更新日期：   2026-1-22
  */
void User_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
	create_task_1();// 创建工作任务1
	create_task_2();// 创建工作任务2

	while(1)
    {		
        task_run(mission_period);               //任务运行
		MY_HMI.HMI_time += mission_period;

		Work_time = Work_time + ((float)mission_period)/1000;
		MY_work_time_update(((float)mission_period)/1000);
		MY_System_transformation(((float)mission_period)/1000);
		
        vTaskDelayUntil(&xLastWakeTime,mission_period);//绝对5ms
    }
}


/**
  * 函数作用：  5ms周期定时器回调函数
  * 参数1：     定时器句柄
  * 返回值：    无
  * 
  * 备注：      基于RTOS的定时器
  * 更新日期：   2025-11-27
  */
void user_200Hz(TimerHandle_t xTimer)
{

    if(User_times_count++ == 200)//1000ms
    {
        User_times_count = 0;
    }
    if(User_times_count % 10 == 0)//50ms
    {
//        ANO_DT_Send_MY_DATA_1();
//        ANO_DT_Send_MY_DATA_3();
//        ANO_DT_SendString(1,"FLAY OK!");
        // Printf_uart1("Uart1\n");
        // Printf_uart2("Uart2\n");
        // Printf_uart4("Uart4\n");
        // Printf_uart5("Uart5\n");
        // Printf_uart6("Uart6\n");

        HMI_Data_Print();
    }
}

