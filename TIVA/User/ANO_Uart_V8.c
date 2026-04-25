#include "ANO_Uart_V8.h"
#include "Drv_Uart.h"
#include "ANO_DT_LX.h"
#include <stdarg.h>


u8 ANO_data_to_send[100] = {0};

void ANO_Send_Data(u8 ID,u16 len,...) //灵活帧
{
	u8 _cnt = 0;
	u8 check_sum1 = 0, check_sum2 = 0;
	int data = 0;
	va_list args;

	ANO_data_to_send[_cnt++] = 0xAB;
    ANO_data_to_send[_cnt++] = 0X01;    //原地址	
    ANO_data_to_send[_cnt++] = 0XFE;    //上位机地址
	ANO_data_to_send[_cnt++] = ID;      //功能码ID
	ANO_data_to_send[_cnt++] = 1;		//数据长度（小位）
	ANO_data_to_send[_cnt++] = 1;		//数据长度

	va_start(args, len);
	for (u8 i = 0; i < len-1; i++)
	{
		data = va_arg(args, int);
		ANO_data_to_send[_cnt++] = data;
		ANO_data_to_send[_cnt++] = data >> 8;
	}
	data = va_arg(args, char);
	ANO_data_to_send[_cnt++] = data;

	ANO_data_to_send[4] = (_cnt - 6);//数据长度
	ANO_data_to_send[5] = (_cnt - 6) >> 8;//数据长度

	
	for(u8 i=0;i<_cnt;i++)
	{
		check_sum1 += ANO_data_to_send[i];
		check_sum2 += check_sum1;
	}
	ANO_data_to_send[_cnt++] = check_sum1;
	ANO_data_to_send[_cnt++] = check_sum2;

	DrvUart5SendBuf(ANO_data_to_send, _cnt);
}

