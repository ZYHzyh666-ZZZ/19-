#include "Drv_biubiu.h"

void Dvr_LAZER_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(ANO_RCC_LAZER_1, ENABLE);
	RCC_AHB1PeriphClockCmd(ANO_RCC_LAZER_2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_LAZER_1;
	GPIO_Init(ANO_GPIO_LAZER_1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ANO_Pin_LAZER_2;
	GPIO_Init(ANO_GPIO_LAZER_2, &GPIO_InitStructure);
	
	GPIO_ResetBits(ANO_GPIO_LAZER_1, ANO_Pin_LAZER_1);
	GPIO_ResetBits(ANO_GPIO_LAZER_2, ANO_Pin_LAZER_2);
}
