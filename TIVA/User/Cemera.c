#include "Cemera.h"
#include "ANO_uart_V8.h"

s8 mod = 0;
s16 width = 0;
s16 location = 0;

u8 cemera_data_get[20] = {0};
u8 sum_data = 0;

void Cemera_DataAnl(u8* data, int data_len)
{
	switch(*data)
	{
		case 'A':
		{
			width = *(data+1)*100 + *(data+2)*10 + *(data+3);
			location = *(data+4)*100 + *(data+5)*10 + *(data+6);
			
			ANO_Send_Data(0xF2,3,(int)width,(int)location,0x01);
		}
		case 'B':
		{
			
		}
		case 'C':
		{
			
		}
		case 'D':
		{
			
		}
	}
}

void Cemera_get_onebyte(u8 data)
{
	static u8 _data_len = 0, _data_cnt = 0;
	static u8 rxstate = 0;

//	DrvUart2SendBuf(&data,1);
	if (rxstate == 0 && data == 0xEF)     //帧头1
    {
        rxstate = 1;
    }
    
	else if (rxstate == 1)                //数据长度      
    {
        sum_data = 0;
        _data_len = data;
        _data_cnt = 0;
        rxstate = 2;
    }

    else if (rxstate == 2)                //数据内容
    {	            
        cemera_data_get[_data_cnt] = data; 
        sum_data += data;
        _data_cnt++;       
        if(_data_cnt >= _data_len)
            rxstate = 3;
    }                 

	else if (rxstate == 3)               //校验和      
    {
        if(data == sum_data)
            rxstate = 4;
        else
            rxstate = 0;
    }
    else if (rxstate == 4 && data == 0xFE)
    {
        // MY_Camera_print("ok\n");

        Cemera_DataAnl(cemera_data_get,_data_len);
        rxstate = 0;
    }
	else 
	{		
		rxstate = 0;	
	}
}
