#ifndef _DRV_BEEP_H_
#define _DRV_BEEP_H_
#include "SysConfig.h"


#define ANO_RCC_BEEP RCC_AHB1Periph_GPIOA
#define ANO_GPIO_BEEP GPIOA
#define ANO_Pin_BEEP GPIO_Pin_5

#define Beep_ON ANO_GPIO_BEEP->BSRRL = ANO_Pin_BEEP
#define Beep_OFF ANO_GPIO_BEEP->BSRRH = ANO_Pin_BEEP

typedef struct {
	
	u16 beep_times;//点亮时间
	u16 beep_cnt;//时间计数
} _beep_st;

extern _beep_st MY_beep;


void DvrBeepInit(void);
void MY_beep_20ms(void);

void MY_BEEP_ON(u16 time);


#endif
