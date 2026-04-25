#ifndef _UART_TIVA_H_
#define _UART_TIVA_H_

#include "SysConfig.h"

typedef struct
{
	s16 Roll;
	s16 Pitch;
	s16 Yaw;
	
	s16 Yaw_begin;
}N100_def;

extern N100_def N100;

void Tiva_Data(void); //处理接收的数据              
void Tiva_Receive_Data(uint8_t data); 

void Tiva_postion_print(s16 X, s16 Y, s16 Z);


#endif
