#include "ALL_include.h"

#define X_POSITION 320 
#define Y_POSITION 240 
#define Z_POSITION 0

#define Width_0_baseline 12
#define Width_1_baseline 12
#define Location_0_baseline 240
#define Location_1_baseline 320

char camera_data[20] = {0};
u8 camera_flag = 0, len = 0, count_data = 0;

s16 width[2] = {0};
s16 location[2] = {0};
u8 rx_flag[2] = {0};

void Camera_Data(void) //处理接收的数据              
{	
	switch(camera_data[0])
	{
		case 0x0A:
		{
			width[0] = camera_data[1]*100 + camera_data[2]*10 + camera_data[3];
			location[0] = camera_data[4]*100 + camera_data[5]*10 + camera_data[6];
            width[0] = Width_0_baseline - width[0];
            location[0] = Location_0_baseline - location[0];

			rx_flag[0] = 1;
            camera_flag = 1;
			break;
		}
		case 0x0B:
		{
			width[1] = camera_data[1]*100 + camera_data[2]*10 + camera_data[3];
			location[1] = camera_data[4]*100 + camera_data[5]*10 + camera_data[6];
            width[1] = Width_1_baseline - width[1];
            location[1] = Location_1_baseline - location[1];

			rx_flag[1] = 1;
			camera_flag = 1;
			break;
		}
		case 0x0C:
		{
			
			break;
		}
	}
	
	if(rx_flag[0])
	{
		rx_flag[0] = 0;
		ANO_DT_Send_MY_DATA(0xF6,2,width[0],location[0]);

	}
	if(rx_flag[1])
	{
		rx_flag[1] = 0;
		ANO_DT_Send_MY_DATA(0xF7,2,width[1],location[1]);
	}

    if(PID1_flag)
    {
        postion_target_z = (s16)MY_fly.C_system.z + PID1_updata(location[0]);
        velocity_target_x = PID2_updata(width[0]);
    }
    if(PID2_flag)
    {
        // postion_target_z = MY_fly.C_system.z + PID1_updata(location[0]);
        velocity_target_y = PID3_updata(location[1]);
        velocity_target_x = PID2_updata(width[1]);
    }
	
}

/*接收Camera传过来的数据（转移寄存器）*/
void Camera_Receive_Data(uint8_t data) 
{
    static u8 state = 0;
    static u8 sum_data = 0;
		// ANO_DT_Send_MY_DATA(0xF2,1,data);

//    DrvUart2SendBuf(&data,1);
    if (state == 0 && data == 0xEF)     //帧头1
    {
        state = 1;
    }
    
	else if (state == 1)                //数据长度      
    {
        sum_data = 0;
        len = data;
        count_data = 0;
        state = 2;
        // ANO_DT_Send_MY_DATA(0xF2,1,data);
    }

    else if (state == 2)                //数据内容
    {	            
        camera_data[count_data] = data; 
        sum_data += data;
        count_data++;       
        if(count_data >= len)
            state = 3;
    }                 

	else if (state == 3)               //校验和      
    {
//        if(data == sum_data)
            state = 4;
//        else
//            state = 0;
    }
    else if (state == 4 && data == 0xFE)
    {
        // MY_Camera_print("ok\n");

        Camera_Data();
        state = 0;
    }
	else 
	{		
		state = 0;	
	}
}

void Camera_2_Receive_Data(uint8_t data) 
{
    static u8 state = 0;
    static u8 sum_data = 0;
		// ANO_DT_Send_MY_DATA(0xF2,1,data);

//    DrvUart6SendBuf(&data,1);
    if (state == 0 && data == 0xEF)     //帧头1
    {
        state = 1;
    }
    
	else if (state == 1)                //数据长度      
    {
        sum_data = 0;
        len = data;
        count_data = 0;
        state = 2;
        // ANO_DT_Send_MY_DATA(0xF2,1,data);

    }

    else if (state == 2)                //数据内容
    {	            
        camera_data[count_data] = data; 
        sum_data += data;
        count_data++;       
        if(count_data >= len)
            state = 3;
    }                 

	else if (state == 3)               //校验和      
    {
//        if(data == sum_data)
            state = 4;
//        else
//            state = 0;
    }
    else if (state == 4 && data == 0xFE)
    {
        // MY_Camera_print("ok\n");

        Camera_Data();
        state = 0;
    }
	else 
	{		
		state = 0;	
	}
}
