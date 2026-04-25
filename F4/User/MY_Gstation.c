#include "MY_Gstation.h"
#include "Drv_Uart.h"
#include "Drv_AnoOf.h"
#include "MY_Camera.h"
#include "Drv_led.h"
#include "MY_fly_fun.h"
#include "My_task.h"
#include "MY_control.h"

MY_Gatestation MY_HMI;

/**
  * 函数作用：  地面站数据处理
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      接收数据格式：P 页码 功能码 参数
  * 更新日期：   2025-11-27
  */
void HMI_Data(void) //处理接收的数据            
{	
    if(MY_HMI.data_in[0] != 'P')
        return;
    if(MY_HMI.data_in[1] == '0')              //页0
    {
        if(MY_HMI.data_in[2] == 'B')             //按键
        {
            MY_HMI.HMI_key[MY_HMI.data_in[3]] = 1;
        }

    }
    else if(MY_HMI.data_in[1] == '1')         //页1
    {
        if(MY_HMI.data_in[2] == 'B')             //按键
        {        
            MY_HMI.HMI_key[4] = MY_HMI.data_in[3] + 1;
        }
    }

}

/**
  * 函数作用：  地面站数据接收
  * 参数1：     （u8）单字节数据
  * 返回值：    无
  * 
  * 备注：      不定长数据接收(上限40)，帧头0xEF，帧尾0xFE
  * 更新日期：   2025-11-27
  */
void HMI_Receive_Data(u8 data) 
{
    static u8 HMI_state = 0;
    static u8 HMI_index = 0;
    if (HMI_state == 0 && data == 0xEF)    //帧头1
    {   
        HMI_state = 1;
        HMI_index = 0;
    }
    else if (HMI_state == 1)
    {
        MY_HMI.data_in[HMI_index++] = data;
        if(data == 0xFE)                    //检测帧尾
        {
            HMI_Data();
            HMI_state = 0;
        }
        else if(HMI_index >= 40)            //数据过长，复位
            HMI_state = 0;
    }
	else 
	{		
		HMI_state = 0;	
	}
}

/**
  * 函数作用：  地面站数据打印
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      切记控制打印频率，避免接收端串口堵塞
  * 更新日期：   2025-11-27
  */
void HMI_Data_Print(void)
{
    static u8 hmi_print_state = 0;

    if(hmi_print_state++ % 2 == 1)
    {    
		if(MY_HMI.HMI_flash_position_WTS == 1)//存储位置标志位
        {
            for(u8 i=0;i < 4;i++)
            {
                MY_HMI_print("#P1X%d=%d\xff\xff\xff",2*i,MY_HMI.position[i].X);
                MY_HMI_print("#P1X%d=%d\xff\xff\xff",2*i+1,MY_HMI.position[i].Y);
            }
			MY_HMI_print("#P1X16=%d\xff\xff\xff",MY_HMI.position[4].X);
            MY_HMI_print("#P1X17=%d\xff\xff\xff",MY_HMI.position[4].Y);
            MY_HMI_print("#P1X18=%d\xff\xff\xff",MY_HMI.position[5].X);
            MY_HMI_print("#P1X19=%d\xff\xff\xff",MY_HMI.position[5].Y);
            MY_HMI_print("#P1X20=%d\xff\xff\xff",MY_HMI.position[6].X);
            MY_HMI_print("#P1X21=%d\xff\xff\xff",MY_HMI.position[6].Y);

            MY_HMI.HMI_flash_position_WTS = 0;
			return;
        }
        if(MY_HMI.HMI_work_WTS == 1)//工作标志位
        {
            MY_HMI_print("#P0T6=%s%d\xff\xff\xff","任务",task_1.step);
            MY_HMI_print("#P0X0=%d\xff\xff\xff",(u16)(task_1.mission_times*100));
            MY_HMI_print("#P0X1=%d\xff\xff\xff",ano_of.of_alt_cm);

            MY_HMI.HMI_work_WTS = 0;
        }
        if(MY_HMI.HMI_position_WTS == 1)//位置标志位
        {
            MY_HMI_print("#P0X2=%d\xff\xff\xff",MY_HMI.NOW_position.X);
            MY_HMI_print("#P0X3=%d\xff\xff\xff",MY_HMI.NOW_position.Y);

            MY_HMI_print("#P0X4=%d\xff\xff\xff",MY_HMI.TARGET_position.X);
            MY_HMI_print("#P0X5=%d\xff\xff\xff",MY_HMI.TARGET_position.Y);

            MY_HMI.HMI_position_WTS = 0;
        }

    }
    else if(hmi_print_state % 2 == 0)
    {
        hmi_print_state = 0;
        
        if(camera_flag != 0)
            MY_HMI_print("P1T8%d\xff\xff\xff",camera_flag);
        if(MY_HMI.HMI_flash_position_WTS == 1)//存储位置标志位
        {
            for(u8 i=0;i < 4;i++)
            {
                MY_HMI_print("#P1X%d=%d\xff\xff\xff",2*i,MY_HMI.position[i].X);
                MY_HMI_print("#P1X%d=%d\xff\xff\xff",2*i+1,MY_HMI.position[i].Y);
            }
            MY_HMI.HMI_flash_position_WTS = 0;
        }
                        // MY_HMI_print("#P1X0=999\xff\xff\xff");

        if(MY_HMI.HMI_other_WTS == 1)//其他标志位
        {
            //其他数据更新
            MY_HMI_print("#P1X16=%d\xff\xff\xff",MY_HMI.position[4].X);
            MY_HMI_print("#P1X17=%d\xff\xff\xff",MY_HMI.position[4].Y);
            MY_HMI_print("#P1X18=%d\xff\xff\xff",MY_HMI.position[5].X);
            MY_HMI_print("#P1X19=%d\xff\xff\xff",MY_HMI.position[5].Y);
            MY_HMI_print("#P1X20=%d\xff\xff\xff",MY_HMI.position[6].X);
            MY_HMI_print("#P1X21=%d\xff\xff\xff",MY_HMI.position[6].Y);

            MY_HMI.HMI_other_WTS = 0;
        }
    }
}

/**
  * 函数作用：  地面站更新点位坐标
  * 参数1：     更新坐标长度
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025-11-27
  */
void HMI_position_updata(int len)
{
    u8 i;
    for(i = 0;i < len;i++)
    {
        MY_HMI.position[i].X = postion_set_x[i];
        MY_HMI.position[i].Y = postion_set_y[i];
    }
    MY_HMI.HMI_flash_position_WTS = 1;
}

/**
  * 函数作用：  地面站其他类型数据更新
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2025-11-27
  */
void HMI_other_updata(void)
{
    MY_HMI.HMI_other_WTS = 1;
}

/**
  * 函数作用：  地面站界面初始化
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      不可一次性发送过多数据，怕免串口堵塞
  * 更新日期：   2025-11-27
  */
void HMI_other_init(void)
{
    MY_HMI_print("#P0T0=%s\xff\xff\xff","任务:");
    MY_HMI_print("#P0T1=%s\xff\xff\xff","高度");
    MY_HMI_print("#P0T2=%s\xff\xff\xff","当前X:"); 
    MY_HMI_print("#P0T3=%s\xff\xff\xff","当前Y:");
    MY_HMI_print("#P0T4=%s\xff\xff\xff","目标X:");
    MY_HMI_print("#P0T5=%s\xff\xff\xff","目标Y:");

    MyDelayMs(50);

/*-----------------------------------------------------------------*/

    MY_HMI_print("#P1T0=%s\xff\xff\xff","X1");
    MY_HMI_print("#P1T1=%s\xff\xff\xff","Y1");
    MY_HMI_print("#P1T2=%s\xff\xff\xff","X2"); 
    MY_HMI_print("#P1T3=%s\xff\xff\xff","Y2");
    MY_HMI_print("#P1T4=%s\xff\xff\xff","X3");
    MY_HMI_print("#P1T5=%s\xff\xff\xff","Y3");
    MY_HMI_print("#P1T6=%s\xff\xff\xff","X4");
    MY_HMI_print("#P1T7=%s\xff\xff\xff","Y4");

    MyDelayMs(50);
	
	
    MY_HMI_print("#P0B0=%s\xff\xff\xff","记录");
    MY_HMI_print("#P0B1=%s\xff\xff\xff","存储");
    MY_HMI_print("#P0B2=%s\xff\xff\xff","读取");
	MY_HMI_print("#P0B3=%s\xff\xff\xff","任务完成");

	
    MY_HMI_print("#P1T8=%s\xff\xff\xff","目标");
    MY_HMI_print("#P1T16=%s\xff\xff\xff","A_X"); 
    MY_HMI_print("#P1T17=%s\xff\xff\xff","A_Y");
//    MY_HMI_print("#P1T18=%s\xff\xff\xff","B_X"); 
//    MY_HMI_print("#P1T19=%s\xff\xff\xff","B_Y");    
//    MY_HMI_print("#P1T20=%s\xff\xff\xff","C_X"); 
//    MY_HMI_print("#P1T21=%s\xff\xff\xff","C_Y");
    MY_HMI_print("#P1B65=%s\xff\xff\xff","激光开");
    MY_HMI_print("#P1B66=%s\xff\xff\xff","激光关");
}

