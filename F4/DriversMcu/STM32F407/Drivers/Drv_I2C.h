#ifndef _DRV_I2C_H_
#define _DRV_I2C_H_
#include "SysConfig.h"

#define READ_ADDR                  0x81	 //A0=GND，A1=GND // R=1, W=0
#define WRITE_ADDR                 0x80

#define Config_Reg                 0x00
#define Shunt_V_Reg                0x01
#define Bus_V_Reg                  0x02
#define Power_Reg                  0x03
#define Current_Reg                0x04
#define Calib_Reg                  0x05
#define Mask_En_Reg                0x06
#define Alert_Reg                  0x07
#define Man_ID_Reg                 0xFE  //0x5449
#define ID_Reg                     0xFF  //0x2260


#define I2C_TIMEOUT     (uint32_t)0x001FFFFF

/***************I2C GPIO定义******************/
#define SOFT_I2C_SCL_PIN         GPIO_Pin_10
#define SOFT_I2C_SDA_PIN         GPIO_Pin_9
#define SOFT_I2C_GPIO_PORT       GPIOE
#define SOFT_I2C_SCL_SOURCE      GPIO_PinSource10
#define SOFT_I2C_SDA_SOURCE      GPIO_PinSource9
#define SOFT_I2C_GPIO_CLK        RCC_AHB1Periph_GPIOE

/*------------------------------------------------------------------------------*/
void Drv_I2C_Init(void);
void INA226_Read(uint16_t* data);


#endif
