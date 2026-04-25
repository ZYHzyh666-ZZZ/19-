#ifndef _MY_F4_UART_H_
#define _MY_F4_UART_H_

#include "McuConfig.h"

typedef struct 
{
    s16 x;
    s16 y;
    s16 z;
}_postion_def;

extern _postion_def Fly_postion;

void F4_get_onebyte(u8 data);
void F4_data_handle(void);	//数据处理函数



#endif
