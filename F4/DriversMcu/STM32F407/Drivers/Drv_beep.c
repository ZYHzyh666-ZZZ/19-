#include "Drv_beep.h"

_beep_st MY_beep;

void DvrBeepInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(ANO_RCC_BEEP, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_BEEP;
	GPIO_Init(ANO_GPIO_BEEP, &GPIO_InitStructure);


	GPIO_ResetBits(ANO_GPIO_BEEP, ANO_Pin_BEEP);
}

void MY_BEEP_ON(u16 time)
{
	MY_beep.beep_times = time;
	MY_beep.beep_cnt = 0;
}

//我的蜂鸣器同步
void MY_beep_20ms(void)
{
	if(MY_beep.beep_cnt > MY_beep.beep_times)
		MY_beep.beep_cnt = MY_beep.beep_times;

	if(MY_beep.beep_cnt < MY_beep.beep_times)
	{
		MY_beep.beep_cnt += 5;
		Beep_ON;
	}
	else
	{
		//OFF
		Beep_OFF;
		MY_beep.beep_cnt = 0;
		MY_beep.beep_times = 0;
	}
}

