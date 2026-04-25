#include "Uart_tiva.h"
#include "ANO_DT_LX.h"
#include "Drv_Uart.h"
#include "Drv_AnoOf.h"
#include "MY_fly_state.h"

N100_def N100 = {0};

u8 Tiva_data_in[100] = {0};

u8 N100_one_data = 0;

#define num(X) ((X==' ')?(0):(X-'0'))

s32 LX_to_N100(s32 X)
{
	if(X - N100.Yaw_begin > 18000)
		return X - N100.Yaw_begin - 36000;
	else if(X - N100.Yaw_begin < -18000)
		return X - N100.Yaw_begin + 36000;
	else
		return X - N100.Yaw_begin;
}

void Tiva_Data(void) //处理接收的数据              
{	
    N100.Pitch = num(Tiva_data_in[0])*10000 + num(Tiva_data_in[1])*1000 + num(Tiva_data_in[2])*100 + num(Tiva_data_in[3])*10 + num(Tiva_data_in[4])*1 - 18000;
    N100.Roll = num(Tiva_data_in[5])*10000 + num(Tiva_data_in[6])*1000 + num(Tiva_data_in[7])*100 + num(Tiva_data_in[8])*10 + num(Tiva_data_in[9])*1 - 9000;
    N100.Yaw = num(Tiva_data_in[10])*10000 + num(Tiva_data_in[11])*1000 + num(Tiva_data_in[12])*100 + num(Tiva_data_in[13])*10 + num(Tiva_data_in[14])*1 - 18000;

	if(N100_one_data == 0 && fc_att.st_data.yaw_x100 != 0)
	{
		N100_one_data = 1;
		N100.Yaw_begin = fc_att.st_data.yaw_x100;
	}	

//	N100.Yaw = N100.Yaw > 18000?N100.Yaw % 18000:N100.Yaw;
	N100.Pitch = -N100.Pitch;
	
	MY_body_state_update((float)N100.Pitch / 100.0f, (float)N100.Roll / 100.0f, (float)N100.Yaw / 100.0f, ano_of.of2_dx_fix, ano_of.of2_dy_fix, 0.0f);
	MY_fly.body.location_z = ano_of.of_alt_cm;

	ANO_DT_Send_MY_DATA(0xF3,1,N100.Yaw);
}

/*接收Tiva传过来的数据（转移寄存器）*/
void Tiva_Receive_Data(uint8_t data) 
{
    static u8 state = 0;
    // DrvUart1SendBuf(&data,1);
    if (state == 0 && data == '@')      //帧头1
    {
        state = 1;
    }
	else if (state > 0 && state <= 15)               //接收数据          
    {
        Tiva_data_in[state - 1] = data;
        state++;
    }
    else if (state == 16 && data == '*')//帧尾
    {
        Tiva_Data();
        state = 0;
    }
	else 
	{		
		state = 0;	
	}
}

void Tiva_postion_print(s16 X, s16 Y, s16 Z)
{
	u8 _cnt = 2;
	u8 data[50] = {0};
	u8 data_sum = 0;
	
	data[0] = 0xFE;		//帧头
	data[1] = 0x01;		//长度
	
	data[_cnt++] = X;
	data[_cnt++] = X >> 8;
	data[_cnt++] = Y;
	data[_cnt++] = Y >> 8;
	data[_cnt++] = Z;
	data[_cnt++] = Z >> 8;
	
	for(u8 i = 2;i < _cnt;i++)
		data_sum += data[i];
	data[_cnt] = data_sum;
	data[_cnt+1] = 0xEF;
	
	data[1] = _cnt - 2;
	
	DrvUart1SendBuf(data,_cnt+2);
}
