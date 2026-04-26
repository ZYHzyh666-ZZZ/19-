#include "ALL_include.h"


float Yaw_target = 0;

My_mission_task task_mission_1[]=
{
    {mission_1_1,2.0f},
    {mission_1_2,3.0f},
    {mission_1_3,3.0f},
	{mission_1_4,1.0f},
	{mission_1_5,2.0f},
	{mission_1_6,2.0f},
	{mission_1_7,15.0f},
	{mission_1_8,2.0f},
	// {mission_1_9,2.0f},//3
	// {mission_1_10,2.0f},//2
	// {mission_1_11,2.0f},//1
	// {mission_1_12,2.0f},//降落点
	// {mission_1_13,2.0f},//降落
    // {mission_1_14,2.0f},//降落
	// {mission_1_15,2.0f},//降落
	// {mission_1_13,2.0f},//降落
};
My_task_state task_1;
u8 start_task1_flag = 0;		//任务开始标志

My_mission_task task_mission_2[]=
{
	{mission_2_1,2.0f},//起飞
	{mission_2_2,3.0f},//旋转
	{mission_2_3,17.0f},//上下
	{mission_2_4,5.0f},//起飞
	{mission_2_5,4.0f},//前后
	{mission_2_6,2.0f},//识别
	{mission_2_7,4.0f},//打击目标
	{mission_2_8,2.0f},//打击
};
My_task_state task_2;
u8 start_task2_flag = 0;		//任务开始标志

u8 takeoff_once = 0;			//任务开始第一次标志位

/*-----------------------------*/

s16 postion_begin_x = 0;			//记录起飞坐标_X
s16 postion_begin_y = 0;			//记录起飞坐标_Y

/**
  * 函数作用：  任务状态检测
  * 参数1：     （u16）系统时间ms
  * 返回值：     无
  * 
  * 备注：      放置任务中调用检测
  * 更新日期：   2025-11-27：无
  * 
  */
void task_check(u16 ms)
{
	if(task_1.finish == 1 || task_2.finish == 1)//完成一次任务音效提示
	{
		MY_BEEP_ON(300);
	}
	if(task_1.finish == 1)
	{
		task_1.finish = 0;
		task_1.mission_times = 0;
		task_1.step++;
	}
	else if(start_task1_flag == 1)				//无锁桨检测
	// else if(start_task1_flag == 1 && fc_sta.unlock_sta == 1)	
		task_1.mission_times = task_1.mission_times + (float)ms/1000;//记录时间

	if(task_2.finish == 1)
	{
		task_2.finish = 0;
		task_2.mission_times = 0;
		task_2.step++;
	}
	else if(start_task2_flag == 1)
		task_2.mission_times = task_2.mission_times + (float)ms/1000;//记录时间
}

/**
  * 函数作用：  任务运行函数
  * 参数1：     （u16）系统时间ms
  * 返回值：     无
  * 
  * 备注：      放置在5ms定时器中调用
  * 更新日期：   2025-11-27：无
  * 
  */
void task_run(u16 ms)//任务运行
{
	task_check(ms);
	if(start_task1_flag == 1)			//任务1开始标志
	{
		if(task_1.step < task_1.task_num)
			task_1.mission[task_1.step].task_mission();
		else
			task_1.step = task_1.task_num;//任务结束
	}

	if(start_task2_flag == 1)			//任务2开始标志
	{
		if(task_2.step < task_2.task_num)
			task_2.mission[task_2.step].task_mission();
		else
			task_2.step = task_2.task_num;//任务结束
	}
}

/**
  * 函数作用：  任务1创建函数
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025-11-27：无
  * 
  */
void create_task_1(void)//创建任务
{
	task_1.mission = task_mission_1;

	task_1.finish = 0;
	task_1.step = 0;
	task_1.task_num = sizeof(task_mission_1)/sizeof(task_mission_1[0]);
	task_1.mission_times = 0;
}

/**
  * 函数作用：  任务2创建函数
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025-11-27：无
  * 
  */
void create_task_2(void)//创建任务
{
	task_2.mission = task_mission_2;

	task_2.finish = 0;
	task_2.step = 0;
	task_2.task_num = sizeof(task_mission_2)/sizeof(task_mission_2[0]);
	task_2.mission_times = 0;
}

void mission_1_1(void)//起飞
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
		Yaw_target = N100.Yaw;
		camera_flag[0] = 0;
		ANO_DT_SendString(1,"mod1_begin");
	}
	Set_High(postion_target_z);
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定

	// if(ano_of.of_alt_cm > Default_height-3 && _time_ok(task_1))//起飞到150cm
	if(camera_flag[0] && _time_ok(task_1))//起飞到150cm
	{
		task_1.finish = 1;
		takeoff_once = 0;
//		MY_Camera_print("OK");
		ANO_DT_SendString(2,"FLAY OK!");
		Move_stop();
		return;
	}
}

u8 PID1_flag = 0;
u8 PID2_flag = 0;

void mission_1_2(void)//定目标
{
	if(takeoff_once == 0)
	{
		// postion_target_z = Default_height;
		PID1_flag = 1;
		takeoff_once = 1;
	}

	// postion_target_z =  MY_fly.C_system.z + PID1_updata(location[0]);

	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	Set_High(postion_target_z);
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,0);
	
	if(_time_ok(task_1) && abs(location[0]) < 7 && abs(width[0]) < 2)
	{
		task_1.finish = 1;
		takeoff_once = 0;
		ANO_DT_SendString(2,"find OK!");
		return;
	}
}

void mission_1_3(void)//巡线
{
	if(takeoff_once == 0)
	{
		// postion_target_z = Default_height;
		camera_flag[1] = 0;
		takeoff_once = 1;
		
	}

	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,-10);
	Set_High(postion_target_z);

	if(task_1.mission_times < 2.0f)//巡线3s内
		camera_flag[1] = 0;
	if(_time_ok(task_1) && camera_flag[1])
	{
		task_1.finish = 1;
		PID1_flag = 0;
		Move_stop();
		takeoff_once = 0;
		ANO_DT_SendString(2,"go OK!");
		return;
	}
}

void mission_1_4(void)//旋转
{
	if(takeoff_once == 0)
	{
		postion_target_z = 146;
		PID2_flag = 1;
		takeoff_once = 1;
	}
	// postion_target_z =  MY_fly.C_system.z + PID1_updata(location[0]);

	Set_High(postion_target_z);
	// Program_Ctrl_User_Set_YAWdps(-5);//旋转速度
	mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	Program_Ctrl_User_Set_HXYcmps(velocity_target_x,velocity_target_y);
		// Program_Ctrl_User_Set_HXYcmps(velocity_target_x,0);
	
	if(_time_ok(task_1) && abs(Angle_calculation(Yaw_target/100.f,MY_fly.body.yaw)) > 90)
	{
		task_1.finish = 1;
		PID2_flag = 0;
		takeoff_once = 0;
		ANO_DT_SendString(2,"zhuan OK!");
		Move_stop();
		return;
	}
}

void mission_1_5(void)//巡线
{
	if(takeoff_once == 0)
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		Move_stop();
		begin_land = 1;		
		takeoff_once = 1;
	}

	Set_High(postion_target_z);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"land_OK!");
		return;
	}



	// if(takeoff_once == 0)
	// {
	// 	// postion_target_z = Default_height;
	// 	Yaw_target = change_angle_range(Yaw_target + 18000);
	// 	camera_flag[1] = 0;
	// 	PID1_flag = 1;
	// 	takeoff_once = 1;
	// }

	// mode_Hold_Yaw(Yaw_target);				//保持航向角稳定
	// Set_High(postion_target_z);
	// Program_Ctrl_User_Set_HXYcmps(velocity_target_x,-10);

	// if(task_1.mission_times < 3.0f)//巡线3s内
	// 	camera_flag[1] = 0;
	// if(_time_ok(task_1) && camera_flag[1])
	// {
	// 	task_1.finish = 1;
	// 	PID1_flag = 0;
	// 	takeoff_once = 0;
	// 	ANO_DT_SendString(2,"find OK!");
	// 	return;
	// }
}

void mission_1_6(void)//降落
{
	if(takeoff_once == 0)
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		begin_land = 1;		
		takeoff_once = 1;
	}

	Set_High(postion_target_z);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"land_OK!");
		return;
	}
}

void mission_1_7(void)//绕另一根杆
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;

	}

	Set_High(postion_target_z);


	if(_time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		ANO_DT_SendString(2,"yuan OK!");
		Move_stop();
		return;
	}
}

void mission_1_8(void)//降落
{
	if(takeoff_once == 0)
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		begin_land = 1;		
		takeoff_once = 1;
	}

	Set_High(postion_target_z);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_1))
	{
		task_1.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"land_OK!");
		return;
	}
}

/*---------------------------------------------------------*/
void mission_2_1(void)//起飞
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		MY_HMI.TARGET_position.Z = postion_target_z;
		takeoff_once = 1;
		ANO_DT_SendString(1,"mod2_begin");
	}
	Set_High(postion_target_z);
	if(ano_of.of_alt_cm > Default_height - 3 && _time_ok(task_2))//起飞到100cm
	{
		task_2.finish = 1;
		takeoff_once = 0;
//		MY_Camera_print("OK");
		ANO_DT_SendString(2,"mod2_FLAY OK!");
		return;
	}
}

void mission_2_2(void)//锁定
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
	}

	Set_High(postion_target_z);

	if(_time_ok(task_2))
	{
		task_2.finish = 1;
		takeoff_once = 0;
		ANO_DT_SendString(2,"move_1 OK!");
		return;
	}
	// Circular_Radius(R_r_error,0); //距离锁定
	// Circular_center(R_l_error);	//角度锁定

	// if(_time_ok(task_2) && abs(R_r_error) < 4 && abs(R_l_error) < 4)
	// {
	// 	task_2.finish = 1;
	// 	takeoff_once = 0;
	// 	ANO_DT_SendString(2,"move_1 OK!");
	// 	return;
	// }
}

void mission_2_3(void)//转
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;

	}

	Set_High(postion_target_z);


	if(_time_ok(task_2))
	{
		task_2.finish = 1;
		takeoff_once = 0;
		ANO_DT_SendString(2,"move_2 OK!");
		return;
	}
}      

void mission_2_4(void)//转到中间位置
{
	if(takeoff_once == 0)
	{
		postion_target_z = Default_height;
		takeoff_once = 1;
	}

	Set_High(postion_target_z);


	if(_time_ok(task_2))
	{
		task_2.finish = 1;
		takeoff_once = 0;
		ANO_DT_SendString(2,"zhongjian OK!");
		Move_stop();
		return;
	}
}

void mission_2_5(void)//降落
{
	if(takeoff_once == 0)
	{
		postion_target_z = 0;
		MY_HMI.TARGET_position.Z = postion_target_z;
		begin_land = 1;		
		takeoff_once = 1;

		Program_Ctrl_User_Set_YAWdps(0);
		Program_Ctrl_User_Set_HXYcmps(0,0);
	}

	Set_High(postion_target_z);
	if(ano_of.of_alt_cm < 25 && _time_ok(task_2))
	{
		task_2.finish = 1;
		takeoff_once = 0;
		FC_Lock();
		ANO_DT_SendString(2,"mod2_land_OK!");
		return;
	}
}

void mission_2_6(void)//右方
{
	// if(takeoff_once == 0)
	// {
	// 	postion_target_z = 100;
	// 	MY_HMI.TARGET_position.Z = postion_target_z;
	// 	takeoff_once = 1;
	// }

	// Set_High(postion_target_z);
	// if(Circle_x != 0 && Circle_y != 0)
	// {
	// 	task_2.finish = 1;
	// 	takeoff_once = 0;
	// 	ANO_DT_SendString(2,"mod2_move_5 OK!");
	// 	return;
	// }
}

void mission_2_7(void)//降落
{
	// if(takeoff_once == 0)
	// {
	// 	postion_target_z = 50;
	// 	MY_HMI.TARGET_position.Z = postion_target_z;
	// 	begin_land = 1;		
	// 	takeoff_once = 1;
	// }

	// Set_High(postion_target_z); 
	// if(ano_of.of_alt_cm < 55 && _time_ok(task_2) && abs(ano_of.of1_dx) < 2 && abs(ano_of.of1_dy) < 2)
	// {
	// 	task_2.finish = 1;
	// 	takeoff_once = 0;
	// 	ANO_DT_SendString(2,"mod2_domn_OK!");
	// 	Program_Ctrl_User_Set_HXYcmps(0,0);
	// 	return;
	// }
}


void mission_2_8(void)//降落
{
	// if(takeoff_once == 0)
	// {
	// 	postion_target_z = 0;
	// 	MY_HMI.TARGET_position.Z = postion_target_z;
	// 	begin_land = 1;		
	// 	takeoff_once = 1;
	// }

	// Set_High(postion_target_z);
	// if(ano_of.of_alt_cm < 25 && _time_ok(task_2))
	// {
	// 	task_2.finish = 1;
	// 	takeoff_once = 0;
	// 	FC_Lock();
	// 	ANO_DT_SendString(2,"mod2_land_OK!");
	// 	return;
	// }
}
