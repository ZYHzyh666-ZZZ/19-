#ifndef _LED_H_
#define _LED_H_

#include "SysConfig.h"

//
#define LED_R_BIT 0x01
#define LED_G_BIT 0x02
#define LED_B_BIT 0x04
//
#define LED_A_BIT 0X08
#define LED_ALL_BIT 0x0f

//宏定义LED操作
#define LED4_OFF ANO_GPIO_LED->BSRRL = ANO_Pin_LED4 //BSRRL   LEVEL_H
#define LED4_ON ANO_GPIO_LED->BSRRH = ANO_Pin_LED4	//L

#define LED1_ON ANO_GPIO_LED->BSRRL = ANO_Pin_LED1
#define LED1_OFF ANO_GPIO_LED->BSRRH = ANO_Pin_LED1
#define LED2_ON ANO_GPIO_LED->BSRRL = ANO_Pin_LED2
#define LED2_OFF ANO_GPIO_LED->BSRRH = ANO_Pin_LED2
#define LED3_ON ANO_GPIO_LED->BSRRL = ANO_Pin_LED3
#define LED3_OFF ANO_GPIO_LED->BSRRH = ANO_Pin_LED3

#define LED_ON_R GPIOC->BSRRL = MY_Pin_LED_R
#define LED_OFF_R GPIOC->BSRRH = MY_Pin_LED_R
#define LED_ON_G GPIOA->BSRRL = MY_Pin_LED_G
#define LED_OFF_G GPIOA->BSRRH = MY_Pin_LED_G
#define LED_ON_B GPIOC->BSRRL = MY_Pin_LED_B
#define LED_OFF_B GPIOC->BSRRH = MY_Pin_LED_B

/***************LED GPIO定义******************/
#define ANO_RCC_LED RCC_AHB1Periph_GPIOE
#define ANO_GPIO_LED GPIOE

#define ANO_Pin_LED4 GPIO_Pin_7
//
#define ANO_Pin_LED1 GPIO_Pin_14 //r
#define ANO_Pin_LED2 GPIO_Pin_13 //g
#define ANO_Pin_LED3 GPIO_Pin_15 //b

#define MY_Pin_LED_R GPIO_Pin_4 //r
#define MY_Pin_LED_G GPIO_Pin_7 //g
#define MY_Pin_LED_B GPIO_Pin_5 //b
/*************LED 数量定义*********************/
#define LED_NUM 4
#define MY_LED_NUM 3

/***********MY_LED应用层**********/
#define MY_LED_OFF() (MY_led.brightness[0] = 0, MY_led.brightness[1] = 0, MY_led.brightness[2] = 0)

typedef struct {
	//
	s8 brightness[LED_NUM];//亮度
	u16 bright_times[LED_NUM];//点亮时间
	u16 bright_cnt[LED_NUM];//时间计数

} _led_st;

extern _led_st led;
extern _led_st MY_led;

void DvrLedInit(void);
void LED_1ms_DRV(void);//凌霄LED同步
void MY_led_light(u16 times);
void MY_LED_ON_R(u8 light, u16 time);
void MY_LED_ON_G(u8 light, u16 time);
void MY_LED_ON_B(u8 light, u16 time);

//void MY_led_on_ms(u8 LED_color,u32 ms_time);//我的LED点亮指定时间


#endif
