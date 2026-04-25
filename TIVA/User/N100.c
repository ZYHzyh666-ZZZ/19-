#include "N100.h"
#include "Drv_Uart.h"
#include "ANO_DT_LX.h"
#include "math.h"

u8 N100_data_get[128];
N100_def MY_N100 = {0};

//#define N100_to_LX(x) (x+81>360?x+81-360:x+81)

double X_V = 0, Y_V = 0, Z_V = 0;

void N100_get_onebyte(u8 data)
{
	static u8 _data_len = 0, _data_cnt = 1;
	static u8 rxstate = 0;

//	DrvUart2SendBuf(&data,1);
	if (rxstate == 0 && data == 0xFC)				//帧头
	{
		rxstate = 1;
		N100_data_get[0] = 0;
	}
	else if (rxstate == 1)	//指令类型   AHRS解算
	{
		if(data == cmd_AHRS || data == cmd_INS)
		{
			rxstate = 2;
			_data_len = 0;
			N100_data_get[0] = data;
		}
		else
		{
			rxstate = 0;
		}
	}
	else if (rxstate == 2)							//数据长度
	{
		rxstate = 3;
		_data_len = data;	
		_data_cnt = 1;
	}
	else if (rxstate == 3 && _data_len > 0)			//流水序号
	{
		rxstate = 4;
	}
	else if (rxstate == 4)							//帧头CRC8
	{
		rxstate = 5;
	}
	else if (rxstate == 5)							//CRC16_H
	{
		rxstate = 6;
	}
	else if (rxstate == 6)							//CRC16_L
	{
		rxstate = 7;
	}
	else if (rxstate == 7)							//data
	{
		if(_data_cnt < _data_len)
		{
			N100_data_get[_data_cnt++] = data;
		}
		else
			rxstate = 8;
	}
	else if (rxstate == 8 && data == 0xFD)			//帧尾
	{
		rxstate = 0;
		N100_DataAnl(N100_data_get, _data_len); //处理数据
	}
	else
	{		
//		DrvUart1SendBuf(&data,1);
//		print_u1("%d\r\n",rxstate);	
		rxstate = 0;

	}
}

void N100_DataAnl(u8* data, int data_len)
{
	switch(*data)
	{
		case cmd_AHRS:
		{
			MY_N100.AHRS.V_Roll	 = Data_Trans(*(data + 1),*(data + 2),*(data + 3),*(data + 4));
			MY_N100.AHRS.V_Pitch = Data_Trans(*(data + 5),*(data + 6),*(data + 7),*(data + 8));
			MY_N100.AHRS.V_Yaw	 = Data_Trans(*(data + 9),*(data + 10),*(data + 11),*(data + 12));
			
			MY_N100.AHRS.Roll	 = Data_Trans(*(data + 13),*(data + 14),*(data + 15),*(data + 16));
			MY_N100.AHRS.Pitch	 = Data_Trans(*(data + 17),*(data + 18),*(data + 19),*(data + 20));
			MY_N100.AHRS.Yaw	 = Data_Trans(*(data + 21),*(data + 22),*(data + 23),*(data + 24));
			
			MY_N100.AHRS.Q1	 = Data_Trans(*(data + 25),*(data + 26),*(data + 27),*(data + 28));
			MY_N100.AHRS.Q2	 = Data_Trans(*(data + 29),*(data + 30),*(data + 31),*(data + 32));
			MY_N100.AHRS.Q3	 = Data_Trans(*(data + 33),*(data + 34),*(data + 35),*(data + 36));
			MY_N100.AHRS.Q4	 = Data_Trans(*(data + 37),*(data + 38),*(data + 39),*(data + 40));
			
				
			MY_N100.AHRS.Roll = MY_N100.AHRS.Roll * 57.3052;	//转化为-180-180
			MY_N100.AHRS.Pitch = MY_N100.AHRS.Pitch * 58.026;	//转化为-90-90
			MY_N100.AHRS.Yaw = MY_N100.AHRS.Yaw * 57.343;		//转化为
			
			MY_N100.AHRS.Yaw = MY_N100.AHRS.Yaw+90 > 180?(MY_N100.AHRS.Yaw+90 - 360):MY_N100.AHRS.Yaw+90;
			
			if(MY_N100.AHRS.Roll != 0 && MY_N100.AHRS.Pitch != 0 && MY_N100.AHRS.Yaw != 0)
				print_F4("@%5d%5d%5d*",(int)((MY_N100.AHRS.Roll+180)*100),(int)((MY_N100.AHRS.Pitch+90)*100),(int)((MY_N100.AHRS.Yaw+180)*100));
			break;
		}
		case cmd_INS:
		{
			MY_N100.INS.Velocity_X	= Data_Trans(*(data + 1),*(data + 2),*(data + 3),*(data + 4));
			MY_N100.INS.Velocity_Y	= Data_Trans(*(data + 5),*(data + 6),*(data + 7),*(data + 8));
			MY_N100.INS.Velocity_Z	= Data_Trans(*(data + 9),*(data + 10),*(data + 11),*(data + 12));
			
			MY_N100.INS.Velocity_X  *= 100;
			MY_N100.INS.Velocity_Y  *= 100;
			MY_N100.INS.Velocity_Z  *= 100;
			
			MY_N100.INS.Acceleration_X	 = Data_Trans(*(data + 13),*(data + 14),*(data + 15),*(data + 16));
			MY_N100.INS.Acceleration_Y	 = Data_Trans(*(data + 17),*(data + 18),*(data + 19),*(data + 20));
			MY_N100.INS.Acceleration_Z	 = Data_Trans(*(data + 21),*(data + 22),*(data + 23),*(data + 24));
			
			MY_N100.INS.Acceleration_X *= 100;
			MY_N100.INS.Acceleration_Y *= 100;
			MY_N100.INS.Acceleration_Z *= 100;
			
			MY_N100.INS.Location_X	 = Data_Trans(*(data + 25),*(data + 26),*(data + 27),*(data + 28));
			MY_N100.INS.Location_Y	 = Data_Trans(*(data + 29),*(data + 30),*(data + 31),*(data + 32));
			MY_N100.INS.Location_Z	 = Data_Trans(*(data + 33),*(data + 34),*(data + 35),*(data + 36));
			
			MY_N100.INS.Location_X  *= 100;
            MY_N100.INS.Location_Y  *= 100;
			MY_N100.INS.Location_Z  *= 100;
			
			MY_N100.INS.Pressure_Altitude	 = Data_Trans(*(data + 57),*(data + 58),*(data + 59),*(data + 60));
			
			// if(MY_N100.AHRS.Roll != 0 && MY_N100.AHRS.Pitch != 0 && MY_N100.AHRS.Yaw != 0)
//			print_u1("----------------------------\r\n");
			print_u1("%f,%f,%f,%f,%f,%f\r\n",MY_N100.INS.Acceleration_X,MY_N100.INS.Acceleration_Y,MY_N100.INS.Acceleration_Z,MY_N100.INS.Location_X,MY_N100.INS.Location_Y,MY_N100.INS.Location_Z);
//			print_u1("%f,%f,%f,%f\r\n",MY_N100.INS.Location_X,MY_N100.INS.Location_Y,MY_N100.INS.Location_Z,MY_N100.INS.Pressure_Altitude);

			break;
		}
		case 0x60:
		{
			X_V	= Data_Trans(*(data + 1),*(data + 2),*(data + 3),*(data + 4));
			Y_V	= Data_Trans(*(data + 5),*(data + 6),*(data + 7),*(data + 8));
			Z_V	= Data_Trans(*(data + 9),*(data + 10),*(data + 11),*(data + 12));

			print_u1("%f,%f,%f\r\n",X_V*100,Y_V*100,Z_V*100);
			break;
		}
		case 0x61:
		{
			X_V	= Data_Trans(*(data + 1),*(data + 2),*(data + 3),*(data + 4));
			Y_V	= Data_Trans(*(data + 5),*(data + 6),*(data + 7),*(data + 8));
			Z_V	= Data_Trans(*(data + 9),*(data + 10),*(data + 11),*(data + 12));

			print_u1("%f,%f,%f\r\n",X_V*100,Y_V*100,Z_V*100);

			break;
		}
	}


}


float Data_Trans(u8 Data_1,u8 Data_2,u8 Data_3,u8 Data_4)
{
	long long transition_32;
	float tmp=0;
	int sign=0;
	int exponent=0;
	float mantissa=0;
	transition_32 = 0;
	transition_32 |=  Data_4<<24;   
	transition_32 |=  Data_3<<16; 
	transition_32 |=  Data_2<<8;
	transition_32 |=  Data_1;
	sign = (transition_32 & 0x80000000) ? -1 : 1;//符号位
	//先右移操作，再按位与计算，出来结果是30到23位对应的e
	exponent = ((transition_32 >> 23) & 0xff) - 127;
	//将22~0转化为10进制，得到对应的x系数 
	mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);
	tmp=sign * mantissa * pow(2, exponent);
	return tmp;
}
