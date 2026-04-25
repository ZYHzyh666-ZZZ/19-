#ifndef _DRV_KEY_H_
#define _DRV_KEY_H
#include "SysConfig.h"

// #define FC_PCB 21
/***************KEY GPIO定义******************/
#if (FC_PCB == 20)
#define ANO_RCC_KEY0 RCC_AHB1Periph_GPIOE
#define ANO_RCC_KEY1 RCC_AHB1Periph_GPIOA
#define ANO_RCC_KEY2 RCC_AHB1Periph_GPIOA
#define ANO_RCC_KEY3 RCC_AHB1Periph_GPIOC
#define ANO_RCC_KEY4 RCC_AHB1Periph_GPIOC

#define ANO_GPIO_KEY0 GPIOE
#define ANO_GPIO_KEY1 GPIOA
#define ANO_GPIO_KEY2 GPIOA
#define ANO_GPIO_KEY3 GPIOC
#define ANO_GPIO_KEY4 GPIOC

#define ANO_Pin_KEY0 GPIO_Pin_7
#define ANO_Pin_KEY1 GPIO_Pin_2
#define ANO_Pin_KEY2 GPIO_Pin_3
#define ANO_Pin_KEY3 GPIO_Pin_8
#define ANO_Pin_KEY4 GPIO_Pin_8
//
#elif (FC_PCB == 21)
#define ANO_RCC_KEY0 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY1 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY2 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY3 RCC_AHB1Periph_GPIOD
// #define ANO_RCC_KEY4 RCC_AHB1Periph_GPIOD

#define ANO_GPIO_KEY0 GPIOD
#define ANO_GPIO_KEY1 GPIOD
#define ANO_GPIO_KEY2 GPIOD
#define ANO_GPIO_KEY3 GPIOD
// #define ANO_GPIO_KEY4 GPIOD

#define ANO_Pin_KEY0 GPIO_Pin_8
#define ANO_Pin_KEY1 GPIO_Pin_9
#define ANO_Pin_KEY2 GPIO_Pin_10
#define ANO_Pin_KEY3 GPIO_Pin_11
// #define ANO_Pin_KEY4 GPIO_Pin_12

#elif (FC_PCB == 22)
#define ANO_RCC_KEY0 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY1 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY2 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY3 RCC_AHB1Periph_GPIOD
//#define ANO_RCC_KEY4 RCC_AHB1Periph_GPIOD

#define ANO_GPIO_KEY0 GPIOD
#define ANO_GPIO_KEY1 GPIOD
#define ANO_GPIO_KEY2 GPIOD
#define ANO_GPIO_KEY3 GPIOD
//#define ANO_GPIO_KEY4 GPIOB

#define ANO_Pin_KEY0 GPIO_Pin_10
#define ANO_Pin_KEY1 GPIO_Pin_11
#define ANO_Pin_KEY2 GPIO_Pin_3
#define ANO_Pin_KEY3 GPIO_Pin_4
//#define ANO_Pin_KEY4 GPIO_Pin_4

#endif
/*********************************************/

#if (FC_PCB == 23)
#define ANO_RCC_KEY0 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY1 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY2 RCC_AHB1Periph_GPIOD
#define ANO_RCC_KEY3 RCC_AHB1Periph_GPIOD

#define ANO_RCC_KEYUP_1 RCC_AHB1Periph_GPIOB
#define ANO_RCC_KEYUP_2 RCC_AHB1Periph_GPIOB
#define ANO_RCC_KEYUP_3 RCC_AHB1Periph_GPIOB
#define ANO_RCC_KEYUP_4 RCC_AHB1Periph_GPIOB

/*---------------------------------------*/
#define ANO_GPIO_KEY0 GPIOD
#define ANO_GPIO_KEY1 GPIOD
#define ANO_GPIO_KEY2 GPIOD
#define ANO_GPIO_KEY3 GPIOD

#define ANO_GPIO_KEYUP_1  GPIOB
#define ANO_GPIO_KEYUP_2  GPIOB
#define ANO_GPIO_KEYUP_3  GPIOB
#define ANO_GPIO_KEYUP_4  GPIOB

/*--------------------------------------*/
#define ANO_Pin_KEY0 GPIO_Pin_8
#define ANO_Pin_KEY1 GPIO_Pin_9
#define ANO_Pin_KEY2 GPIO_Pin_10
#define ANO_Pin_KEY3 GPIO_Pin_11

#define ANO_Pin_KEYUP_1 GPIO_Pin_12
#define ANO_Pin_KEYUP_2 GPIO_Pin_13
#define ANO_Pin_KEYUP_3 GPIO_Pin_14
#define ANO_Pin_KEYUP_4 GPIO_Pin_15

/*--------------------------------------*/
#define KEY0_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY0,ANO_Pin_KEY0)
#define KEY1_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY1,ANO_Pin_KEY1)
#define KEY2_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY2,ANO_Pin_KEY2)
#define KEY3_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY3,ANO_Pin_KEY3)

#define KEYUP_1_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEYUP_1,ANO_Pin_KEYUP_1)
#define KEYUP_2_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEYUP_2,ANO_Pin_KEYUP_2)
#define KEYUP_3_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEYUP_3,ANO_Pin_KEYUP_3)
#define KEYUP_4_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEYUP_4,ANO_Pin_KEYUP_4)

#endif

/*---------------------------------------*/
#define ANO_RCC_KEY1 RCC_AHB1Periph_GPIOE
#define ANO_RCC_KEY2 RCC_AHB1Periph_GPIOE
#define ANO_RCC_KEY3 RCC_AHB1Periph_GPIOA
#define ANO_RCC_KEY4 RCC_AHB1Periph_GPIOA

/*---------------------------------------*/
#define ANO_GPIO_KEY1 GPIOE
#define ANO_GPIO_KEY2 GPIOE
#define ANO_GPIO_KEY3 GPIOA
#define ANO_GPIO_KEY4 GPIOA

/*--------------------------------------*/
#define ANO_Pin_KEY1 GPIO_Pin_3
#define ANO_Pin_KEY2 GPIO_Pin_2
#define ANO_Pin_KEY3 GPIO_Pin_11
#define ANO_Pin_KEY4 GPIO_Pin_12

/*--------------------------------------*/
#define KEY1_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY1,ANO_Pin_KEY1)
#define KEY2_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY2,ANO_Pin_KEY2)
#define KEY3_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY3,ANO_Pin_KEY3)
#define KEY4_VALUE  GPIO_ReadInputDataBit(ANO_GPIO_KEY4,ANO_Pin_KEY4)

typedef struct
{
	u8 Key1_Press;
	u8 Key2_Press;
	u8 Key3_Press;
	u8 Key4_Press;
	u8 Key5_Press;
}_Key_Value;


extern _Key_Value key_value;

void Drv_key_Init(void);
void Key_Scan(void);//按键扫描函数
void KEY_updata(void);//按键状态更新

#endif
