/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作��1�7    ：匿名科刄1�7
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 抢�术Q羄1�7 ＄1�7190169595
 * 描述    ：LED驱动
**********************************************************************************/
#include "Drv_led.h"



void DvrLedInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(ANO_RCC_LED, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_LED1 | ANO_Pin_LED2 | ANO_Pin_LED3;
	GPIO_Init(ANO_GPIO_LED, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MY_Pin_LED_B | MY_Pin_LED_R;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MY_Pin_LED_G;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_LED4;
	GPIO_Init(ANO_GPIO_LED, &GPIO_InitStructure);

	GPIO_SetBits(ANO_GPIO_LED, ANO_Pin_LED1);
	GPIO_SetBits(ANO_GPIO_LED, ANO_Pin_LED2);
	GPIO_SetBits(ANO_GPIO_LED, ANO_Pin_LED3);

	GPIO_ResetBits(GPIOC, MY_Pin_LED_R);
	GPIO_ResetBits(GPIOC, MY_Pin_LED_B);
	GPIO_ResetBits(GPIOA, MY_Pin_LED_G);

}
/*开启凌霄同步LED*/
/*按位开启 0B0001 0B0010 0B0100 0B1000*/
void LED_On_Off(uint16_t leds)
{
	if (leds & LED_R_BIT)
		LED1_ON;
	else
		LED1_OFF;

	if (leds & LED_G_BIT)
		LED2_ON;
	else
		LED2_OFF;

	if (leds & LED_B_BIT)
		LED3_ON;
	else
		LED3_OFF;

	if (leds & LED_A_BIT)
		LED4_ON;
	else
		LED4_OFF;
}

//凌霄LED的同步，1ms调用一次
/*ͨ凌霄led数据保存在led.brightness中*/
_led_st led;
void LED_1ms_DRV() //
{
	static u16 led_cnt[LED_NUM];
	u16 led_tmp;
	for (u8 i = 0; i < LED_NUM; i++)
	{
		if (led_cnt[i] < (s16)led.brightness[i])
		{
			//ON
			led_tmp |= (1 << i);
		}
		else
		{
			//OFF
			led_tmp &= ~(1 << i);
		}

		if (++led_cnt[i] >= 20)//亮度调节0-20
		{
			led_cnt[i] = 0;
		}
	}
	
	LED_On_Off(led_tmp);
}

/*开启我的LED*/
/*按位开启 0B0001 0B0010 0B0100 0B1000*/
void MY_LED_On_Off(uint16_t leds)
{
	if (leds & LED_R_BIT)
		LED_ON_R;
	else
		LED_OFF_R;

	if (leds & LED_G_BIT)
		LED_ON_G;
	else
		LED_OFF_G;

	if (leds & LED_B_BIT)
		LED_ON_B;
	else
		LED_OFF_B;
}

_led_st MY_led;

void MY_LED_ON_R(u8 light, u16 time)
{
	MY_led.brightness[0] = light;
	MY_led.bright_times[0] = time;
	MY_led.bright_cnt[0] = 0;
}
void MY_LED_ON_G(u8 light, u16 time)
{
	MY_led.brightness[1] = light;
	MY_led.bright_times[1] = time;
	MY_led.bright_cnt[1] = 0;
}
void MY_LED_ON_B(u8 light, u16 time)
{
	MY_led.brightness[2] = light;
	MY_led.bright_times[2] = time;
	MY_led.bright_cnt[2] = 0;
}

/**
  * 函数作用：  我的LED显示
  * 参数1：     时间间隔（ms）
  * 返回值：    无
  * 
  * 备注：      基于RTOS的外设调用函数的LED显示
  * 更新日期：   2025-9
  * 			2026-1-22	由固定时间间隔调用改为参数传入时间间隔
  */
void MY_led_light(u16 times)
{
	static u16 led_cnt[MY_LED_NUM];
	u16 led_tmp;
	for (u8 i = 0; i < MY_LED_NUM; i++)									//遍历所有LED
	{
		if(MY_led.bright_cnt[i] < MY_led.bright_times[i])
		{
			if (led_cnt[i] < (s16)MY_led.brightness[i])
				led_tmp |= (1 << i);//ON
			else
				led_tmp &= ~(1 << i);//OFF

			if (++led_cnt[i] >= 4)//亮度调节0-5
				led_cnt[i] = 0;
			MY_led.bright_cnt[i] += times;
		}
		else
		{
			led_tmp &= ~(1 << i);//OFF
			MY_led.bright_cnt[i] = 0;
			MY_led.bright_times[i] = 0;
		}
	}
	MY_LED_On_Off(led_tmp);
}

/******************* (C) COPYRIGHT 2016 ANO TECH *****END OF FILE************/
