#ifndef _MY_CAMERA_H_
#define _MY_CAMERA_H_

#include "SysConfig.h"

extern u8 camera_flag;

void Camera_Data(void); //处理接收的数据              
void Camera_Receive_Data(uint8_t data); 
void Camera_2_Receive_Data(uint8_t data); 


#endif
