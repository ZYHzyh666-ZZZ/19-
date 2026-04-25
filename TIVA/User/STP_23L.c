#include "STP_23L.h"
#include "Drv_Uart.h"


#define HEADER 0xAA							/* 起始符 */
#define device_address 0x00     /* 设备地址 */
#define chunk_offset 0x00       /* 偏移地址命令 */
#define PACK_GET_DISTANCE 0x02 	/* 获取测量数据命令 */
#define PACK_RESET_SYSTEM 0x0D 	/* 复位命令 */
#define PACK_STOP 0x0F 				  /* 停止测量数据传输命令 */
#define PACK_ACK 0x10           /* 应答码命令 */
#define PACK_VERSION 0x14       /* 获取传感器信息命令 */

#define state	STP23L_data_get[(STP23L_state - 10) % 256]

u8 STP23L_data_get[512];

LidarPointTypedef Pack_Data[12];/* 雷达接收的数据储存在这个变量之中 */
LidarPointTypedef Pack_sum;     /* 输出结果储存 */

uint8_t STP23L_state = 10;		//总位数
uint8_t STP23L_state_num = 0;	//分位数
uint8_t STP23L_cnt = 0;			
uint32_t STP23L_temp_data = 0;	//校验位

uint16_t count = 0;
uint32_t sum = 0;
uint8_t cnt = 0;

uint8_t confidence;
int16_t STP23L_distance,noise,reftof;
uint16_t peak,intg;

u8 data_state = 0xFF, STP23L_state_flag = 1;
u8 rxstate = 0;

void STP23L_get_onebyte(u8 data)
{
	static u16 data_len = 0;
//									print_u1("%d\r\n",rxstate);

	if (rxstate == 0 && data == 0xAA)				//帧头
	{
		rxstate = 1;
		data_state = 0xFF;
	}
	else if (rxstate == 1 && data == 0xAA)				//帧头
	{
		rxstate = 2;

	}
	else if (rxstate == 2 && data == 0xAA)				//帧头
	{
		rxstate = 3;

	}
	else if (rxstate == 3 && data == 0xAA)				//帧头
	{
		rxstate = 4;

	}
	else if (rxstate == 4 && data == device_address)//验证地址
	{
		rxstate = 5;
		
	}
	else if (rxstate == 5)							//数据指令
	{
		rxstate = 6;
		data_state = 0xFF;
		switch(data)
		{
			case PACK_GET_DISTANCE:		//测量距离指令
			{
				data_state = PACK_GET_DISTANCE;
				break;
			}
			case PACK_RESET_SYSTEM:		//复位指令
			{
				data_state = PACK_RESET_SYSTEM;
				break;
			}
			case PACK_STOP:				//停止测量指令
			{
				data_state = PACK_STOP;
				break;
			}
			case PACK_ACK:				//应答码指令
			{
				data_state = PACK_ACK;
				break;
			}
			case PACK_VERSION:			//获取传感器信息指令
			{
				data_state = PACK_VERSION;
				break;
			}
			default:
			{
				rxstate = 0;
				break;
			}	
		}
		
	}
	else if (rxstate == 6 && data == chunk_offset)	//校验
	{
		rxstate = 7;
	}
	else if (rxstate > 6 && rxstate < 10)			//无效
	{
		rxstate += 1;
	}
	else if (rxstate == 10)							//开始记录有效数据
	{
		STP23L_state_flag = 0;
		data_len = 0;
		rxstate += 1;
		STP23L_data_get[data_len++] = data;

	}
	else if(rxstate > 10 && (data_state == PACK_GET_DISTANCE) && (STP23L_state_flag == 0))
	{
		if(rxstate <= 190)
		{
			rxstate++; 
			STP23L_data_get[data_len++] = data;
		}
		else if(rxstate < 192)
			rxstate++; 
		else
		{
			STP23L_DataAnl();
			STP23L_state_flag = 1;
			data_len = 0;
			rxstate = 0;
			data_state = 0xFF;
		}
	}
	else
	{		

		rxstate = 0;
	}
//	print_u1("data_state=%x,STP23L_state_flag=%x\r\n",data_state,STP23L_state_flag);

}

void STP23L_DataAnl(void)
{
	if(STP23L_state > 9)
	{
		for(;STP23L_state < 190;)
		{
			STP23L_state_num = (STP23L_state - 10)%15;
			switch(STP23L_state_num)
			{
					case 0: 
					{
						Pack_Data[STP23L_cnt].distance = (uint16_t)state ;				 /* 距离数据低八位 */
						STP23L_state++;
						break;
					}							
					case 1:
					{							
						Pack_Data[STP23L_cnt].distance = ((uint16_t)state<<8) + Pack_Data[STP23L_cnt].distance;	 /* 距离数据 */
						STP23L_state++;
						break; 
					}
					case 2:
					{
//						Pack_Data[STP23L_cnt].noise = (uint16_t)state;				 /* 环境噪音低八位 */
						STP23L_state++;
						break; 
					}
					case 3:
					{
//						Pack_Data[STP23L_cnt].noise = ((uint16_t)state<<8) + Pack_Data[STP23L_cnt].noise;				 /* 环境噪音 */
						STP23L_state++;
						break; 
					}
					case 4:
					{
//						Pack_Data[STP23L_cnt].peak = (uint32_t)state;				 										 /* 接受强度信息低八位 */
						STP23L_state++;
						break; 
					}
					case 5:
					{
//						Pack_Data[STP23L_cnt].peak = ((uint32_t)state<<8) + Pack_Data[STP23L_cnt].peak;
						STP23L_state++;
						break; 
					}
					case 6:
					{
//						Pack_Data[STP23L_cnt].peak = ((uint32_t)state<<16) + Pack_Data[STP23L_cnt].peak;	
						STP23L_state++;
						break; 
					}
					case 7:
					{
//						Pack_Data[STP23L_cnt].peak = ((uint32_t)state<<24) + Pack_Data[STP23L_cnt].peak;				    /* 接受强度信息 */
						STP23L_state++;
						break; 
					}
					case 8:
					{
						Pack_Data[STP23L_cnt].confidence = state;				 /* 置信度 */
						STP23L_state++;
						break; 
					}
					case 9:
					{
//						Pack_Data[STP23L_cnt].intg = (uint32_t)state;															/* 积分次数低八位 */
						STP23L_state++;
						break; 
					}
					case 10:
					{
//						Pack_Data[STP23L_cnt].intg = ((uint32_t)state<<8) + Pack_Data[STP23L_cnt].intg;
						STP23L_state++;
						break; 
					}
					case 11:
					{
//						Pack_Data[STP23L_cnt].intg = ((uint32_t)state<<16) + Pack_Data[STP23L_cnt].intg;
						STP23L_state++;
						break; 
					}
					case 12:
					{
//						Pack_Data[STP23L_cnt].intg = ((uint32_t)state<<24) + Pack_Data[STP23L_cnt].intg;				  	 /* 积分次数 */
						STP23L_state++;
						break; 
					}
					case 13:
					{
//						Pack_Data[STP23L_cnt].reftof = (int16_t)state;				 								 /* 温度表征值低八位 */
						STP23L_state++;
						break; 
					}
					case 14:
					{
//						Pack_Data[STP23L_cnt].reftof = ((int16_t)state<<8) +Pack_Data[STP23L_cnt].reftof;			/* 温度表征值 */
						STP23L_state++;
						STP23L_cnt++;							 /* 进入下一个测量点 */
						break; 
					}
					default: break;
			}
		}
		STP23L_state = 10;
		STP23L_data_process();  	 /* 数据处理函数，完成一帧之后可进行数据处理 */
		STP23L_cnt = 0; 							 /* 复位*/
	}	
}

void STP23L_data_process(void)
{
	/* 计算距离 */
		uint8_t i;
		LidarPointTypedef Pack_sum;
		count = 0;
		for(i=0;i<12;i++)									/* 12个点取平均 */
		{
			if((Pack_Data[i].distance != 0))  /* 去除0的点 */
			{
				count++;
				Pack_sum.distance += Pack_Data[i].distance;
//				Pack_sum.noise += Pack_Data[i].noise;
//				Pack_sum.peak += Pack_Data[i].peak;
//				Pack_sum.confidence += Pack_Data[i].confidence;
//				Pack_sum.intg += Pack_Data[i].intg;
//				Pack_sum.reftof += Pack_Data[i].reftof;
			}
		}
		if(count !=0)
		{
			STP23L_distance = Pack_sum.distance/count;
			if((STP23L_distance - Pack_Data[0].distance) < -20 || (STP23L_distance - Pack_Data[0].distance) > 20)
				STP23L_distance = Pack_Data[0].distance;
//			noise = Pack_sum.noise/count;
//			peak = Pack_sum.peak/count;
//			confidence = Pack_sum.confidence/count;
//			intg = Pack_sum.intg/count;
//			reftof = Pack_sum.reftof/count;
			Pack_sum.distance = 0;
			Pack_sum.noise = 0;
			Pack_sum.peak = 0;
			Pack_sum.confidence = 0;
			Pack_sum.intg = 0;
			Pack_sum.reftof = 0;
			count = 0;
		}
}


