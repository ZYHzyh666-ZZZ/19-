#include "LX_FC_Fun.h"
#include "LX_FC_State.h"
#include "ANO_DT_LX.h"
#include "Drv_led.h"
#include "Ano_Math.h"
#include "Drv_beep.h"

/*==========================================================================
 * 描述    ：凌霄飞控基本功能主程序
 * 更新时间：2020-03-31 
 * 作者		 ：匿名科创-Jyoun
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 项目合作：18084888982，18061373080
============================================================================
 * 匿名科创团队感谢大家的支持，欢迎大家进群互相交流、讨论、学习。
 * 若您觉得匿名有不好的地方，欢迎您拍砖提意见。
 * 若您觉得匿名好，请多多帮我们推荐，支持我们。
 * 匿名开源程序代码欢迎您的引用、延伸和拓展，不过在希望您在使用时能注明出处。
 * 君子坦荡荡，小人常戚戚，匿名坚决不会请水军、请喷子，也从未有过抹黑同行的行为。  
 * 开源不易，生活更不容易，希望大家互相尊重、互帮互助，共同进步。
 * 只有您的支持，匿名才能做得更好。  
===========================================================================*/

//////////////////////////////////////////////////////////////////////
//基本功能函数
//////////////////////////////////////////////////////////////////////
//
u8 FC_Unlock()
{
	//
	fc_sta.unlock_cmd = 1; //解锁
	//按协议发送指令
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x01;
		CMD_Send(0XFF, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//
u8 FC_Lock()//紧急锁机
{
	//
	fc_sta.unlock_cmd = 0; //上锁
	//按协议发送指令
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x02;
		CMD_Send(0XFF, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//改变飞控模式(模式0-3)发送给凌霄IMU
u8 LX_Change_Mode(u8 new_mode)
{
	static u8 old_mode;
	if (old_mode != new_mode)
	{
		
		if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
		{
			old_mode = fc_sta.fc_mode_cmd = new_mode;
			//按协议发送指令
			dt.cmd_send.CID = 0X01;
			dt.cmd_send.CMD[0] = 0X01;
			dt.cmd_send.CMD[1] = 0X01;
			dt.cmd_send.CMD[2] = fc_sta.fc_mode_cmd;
			CMD_Send(0xff, &dt.cmd_send);
			//模式转换提示
			MY_LED_ON_G(4,100);
			return 1;
		}
		else
		{
			return 0;
		}

	}
	else //已经在当前模式
	{
		return 1;
	}
}

//一键返航
//需要注意，程控模式下才能执行返航
u8 OneKey_Return_Home()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X07;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}


//一键起飞(高度cm)
u8 OneKey_Takeoff(u16 height_cm)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//一键降落
u8 OneKey_Land()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X06;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//平移(距离cm，速度cmps，方向角度0-360度)
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X03;
		//
		dt.cmd_send.CMD[2] = BYTE0(distance_cm);
		dt.cmd_send.CMD[3] = BYTE1(distance_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		dt.cmd_send.CMD[6] = BYTE0(dir_angle_0_360);
		dt.cmd_send.CMD[7] = BYTE1(dir_angle_0_360);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//水平校准
u8 Horizontal_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X03;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//磁力计校准
u8 Mag_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X04;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//6面加速度校准
u8 ACC_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//陀螺仪校准
u8 GYR_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X02;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************************************************
以下函数，仅在定点模式下生效。
**********************************************************************************************************/
#define MAX_PC_XYVEL_CMPS  200
#define MAX_PC_ZVEL_CMPS   150
#define MAX_PC_PAL_DPS     100
/**********************************************************************************************************
*函 数 名: Program_Ctrl_User_Set_HXYcmps
*功能说明: 程控功能，航向水平坐标系下速度设定（实时控制）
*参    数: X速度（厘米每秒，正为前进，负为后退，Y速度（厘米每秒，正为左移，负为右移）
*返 回 值: 无
**********************************************************************************************************/
void Program_Ctrl_User_Set_HXYcmps(s16 hx_vel_cmps,s16 hy_vel_cmps)
{
	if(fc_sta.fc_mode_sta == 2)//定点模式
	{	
		rt_tar.st_data.vel_x = hx_vel_cmps;
		rt_tar.st_data.vel_y = hy_vel_cmps;
//		length_limit(&hx_vel_cmps,&hy_vel_cmps,MAX_PC_XYVEL_CMPS,&(rt_tar.st_data.vel_x),&(rt_tar.st_data.vel_y));
	}
	else
		return;
}
/**********************************************************************************************************
*函 数 名: Program_Ctrl_User_Set_WHZcmps
*功能说明: 程控功能，上升下降速度设定（实时控制）
*参    数: 速度（厘米每秒，正为上升，负为下降）
*返 回 值: 无
**********************************************************************************************************/
void Program_Ctrl_User_Set_Zcmps(s16 z_vel_cmps)
{
	if(fc_sta.fc_mode_sta == 2)//定点模式
	{
		rt_tar.st_data.vel_z = z_vel_cmps;
		rt_tar.st_data.vel_z =LIMIT(z_vel_cmps,-MAX_PC_ZVEL_CMPS,MAX_PC_ZVEL_CMPS);
	}
	else
		return;
}
/**********************************************************************************************************
*函 数 名: Program_Ctrl_User_Set_YAWdps
*功能说明: 程控功能，航向速度设定（实时控制）
*参    数: 速度（度每秒，正为右转，负为左转）
*返 回 值: 无
**********************************************************************************************************/
void Program_Ctrl_User_Set_YAWdps(s16 yaw_pal_dps)
{
	if(fc_sta.fc_mode_sta == 2)//定点模式
	{
		rt_tar.st_data.yaw_dps = -yaw_pal_dps;
		rt_tar.st_data.yaw_dps = LIMIT(rt_tar.st_data.yaw_dps,-MAX_PC_PAL_DPS,MAX_PC_PAL_DPS);
	}
	else
		return;
}

void Rt_tar_Stop(void)//实时控制速度归零
{
	if(fc_sta.fc_mode_sta == 2)//定点模式
	{
		rt_tar.st_data.vel_x   = 0;
		rt_tar.st_data.vel_y   = 0;	
		rt_tar.st_data.vel_z   = 0;
		rt_tar.st_data.yaw_dps = 0;
	}
	else
		return;	
}
