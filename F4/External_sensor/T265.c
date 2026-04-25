#include "T265.h"
#include "string.h"
#include "Drv_Uart.h"
#include "ANO_DT_LX.h"
#include "FreeRTOS.h"
#include "Drv_Sys.h"
#include "Drv_led.h"
#include "MY_Gstation.h"
#include "LX_FC_EXT_Sensor.h"

/*当前位置（毫米）*/
s16 postion_now_x = 0;
s16 postion_now_y = 0;
s16 postion_now_z = 0;

int speed_x = 0;
int speed_y = 0;
int speed_z = 0;

int picth = 0;
int roll = 0;
int yaw = 0;

/*数据缓存区及其他标志位*/
u8 T265_data[30];                                   

u8 T265_Start_Flag = 0;

T265 MY_t265;

u16 data_sum;
u16 data_sum_last;

void T265_check(void)
{
	MY_t265.of_t265_update_cnt_last = MY_t265.of_t265_update_cnt;
}

void T265_Data(void) //处理接收的数据              //  
{	
    u8 i;
    // u8 data_sum;
    data_sum_last = data_sum;
	data_sum = 0;
    /*X位置坐标      毫米*/
    MY_t265.x_coordinate = T265_data[0]*256 + T265_data[1]; 
    MY_t265.x_coordinate -= 10000;

    /*Y位置坐标*/
    MY_t265.y_coordinate = T265_data[2]*256 + T265_data[3]; 
    MY_t265.y_coordinate -= 10000;

    /*Z位置坐标*/
    MY_t265.z_coordinate = T265_data[4]*256 + T265_data[5]; 
    MY_t265.z_coordinate -= 10000;

    /*X轴速度    毫米/秒*/
    MY_t265.vx_coordinate = T265_data[6]*256 + T265_data[7]; 
    MY_t265.vx_coordinate -= 10000; 

    /*Y轴速度*/
    MY_t265.vy_coordinate = T265_data[8]*256 + T265_data[9]; 
    MY_t265.vy_coordinate -= 10000; 

    /*Z轴速度*/
    MY_t265.vz_coordinate = T265_data[10]*256 + T265_data[11]; 
    MY_t265.vz_coordinate -= 10000; 

    for(i = 0;i < 12;i++)
        data_sum += T265_data[i];

    if((u8)data_sum != T265_data[12])
        return;
    /*T265安装方向*/

    postion_now_y = MY_t265.y_coordinate;
    postion_now_x = MY_t265.x_coordinate;
    postion_now_z = MY_t265.z_coordinate;  
	
	MY_t265.vx_coordinate = MY_t265.vz_coordinate/10;
	MY_t265.vy_coordinate = MY_t265.vy_coordinate/10;
	MY_t265.vz_coordinate = MY_t265.vx_coordinate/10;
	
    if(data_sum != data_sum_last)
    {
        MY_t265.of_t265_update_cnt++;
        MY_t265.t265_work_sta = 1;
        MY_HMI.NOW_position.X = postion_now_x;
        MY_HMI.NOW_position.Y = postion_now_y;
        MY_HMI.HMI_position_WTS = 1;
//		EXT_choose = 1;

    } 
    else
    {
        MY_t265.t265_work_sta = 0;
    }

    if(data_sum == 0 && data_sum_last == 0)
        MY_t265.t265_link_sta = 0;
    else
        MY_t265.t265_link_sta = 1;
}

/*接收T265传过来的数据（转移寄存器）*/
void T265_Receive_Data(uint8_t data) 
{
    static u8 state = 0;
    if (state == 0 && data == 0xAE)     //帧头1
    {
        state = 1;
    }
    
    else if (state == 1 && data == 0xEA) //帧头2 
    {
        if(T265_Start_Flag == 0)
        {
            T265_Start_Flag = 1;
        }
        state = 2;
    }
/*------------------------------------------------*/
	else if (state == 2)                 // x/256
    {
        state = 3;
        T265_data[0] = data; 
    }
	else if (state == 3)                 // x  
    {
        state = 4;
        T265_data[1] = data; 
    }

/*--------------------------------------------------*/
	else if (state == 4)                 // y/256
    {
        state = 5;
        T265_data[2] = data; 
    }
	else if (state == 5)                 // y 
    {
        state = 6;
        T265_data[3] = data; 
    }

/*--------------------------------------------------*/
	else if (state == 6)                 // z/256
    {
        state = 7;
        T265_data[4] = data; 
    }
	else if (state == 7)                 // z 
    {
        state = 8;
        T265_data[5] = data; 
    }

/*--------------------------------------------------*/
	else if (state == 8)                 // vx/256
    {
        state = 9;
        T265_data[6] = data; 
    }
	else if (state == 9)                 // vx
    {
        state = 10;
        T265_data[7] = data; 
    }

/*--------------------------------------------------*/
	else if (state == 10)                 // vy/256
    {
        state = 11;
        T265_data[8] = data; 
    }
	else if (state == 11)                 // vy 
    {
        state = 12;
        T265_data[9] = data; 
    }

/*--------------------------------------------------*/
	else if (state == 12)                 // vz/256
    {
        state = 13;
        T265_data[10] = data; 
    }
	else if (state == 13)                 // vz 
    {
        state = 14;
        T265_data[11] = data; 
    }
/*--------------------------------------------------*/
	else if (state == 14)                 // 校验
    {
        state = 15;
        T265_data[12] = data; 
    }

/*--------------------------------------------------*/
    else if (state == 15 && data == 0xEF)
    {
        state = 16;
    }
    else if (state == 16 && data == 0xFE)
    {
        T265_Data();
//						ANO_DT_Send_MY_DATA_1();

        state = 0;
    }
	else 
	{		
		state = 0;	
	}
}

