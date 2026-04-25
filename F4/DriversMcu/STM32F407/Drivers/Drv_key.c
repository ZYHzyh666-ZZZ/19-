#include "Drv_key.h"
#include "McuConfig.h"
#include "ANO_DT_LX.h"
#include "T265.h"
#include "Drv_flash.h"
#include "Drv_beep.h"
#include "MY_Gstation.h"
#include "Drv_biubiu.h"
#include "Drv_Uart.h"

#include "MY_fly_fun.h"

/****·É¿Ø°´¼ü******/
_Key_Value key_value = {0,0,0,0,0};

//ÆÕÍ¨¿ª¹Ø
void Drv_key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(ANO_RCC_KEY1, ENABLE);
	RCC_AHB1PeriphClockCmd(ANO_RCC_KEY2, ENABLE);
	RCC_AHB1PeriphClockCmd(ANO_RCC_KEY3, ENABLE);
	RCC_AHB1PeriphClockCmd(ANO_RCC_KEY4, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_KEY1;
	GPIO_Init(ANO_GPIO_KEY1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_KEY2;
	GPIO_Init(ANO_GPIO_KEY2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_KEY3;
	GPIO_Init(ANO_GPIO_KEY3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ANO_Pin_KEY4;
	GPIO_Init(ANO_GPIO_KEY4, &GPIO_InitStructure);
}
//²¦Âë¿ª¹Ø


void Key_Scan(void)//°´¼üÉ¨Ãèº¯Êý
{
    static u8 press_flag[5] = {1,1,1,1,1};		//ËÉÊÖ¼ì²â
	u8 key = 0;
/*-------------------------------*/
	key = KEY1_VALUE;
	if((key == 1 && !press_flag[0]) || MY_HMI.HMI_key[0] == 1)
	{
        key_value.Key1_Press = 1;
		MY_BEEP_ON(50);

		ANO_DT_SendString(Str_RED,"KEY1 OK!");
	}
	press_flag[0] = key;
/*-------------------------------*/
	key = KEY2_VALUE;
	if((key == 1 && !press_flag[1]) || MY_HMI.HMI_key[1] == 1)
	{
        key_value.Key2_Press = 1;
		MY_BEEP_ON(50);

		ANO_DT_SendString(Str_RED,"KEY2 OK!");
	}
	press_flag[1] = key;
/*------------------------------*/
	key = KEY3_VALUE;
	if((key == 1 && !press_flag[2]) || MY_HMI.HMI_key[2] == 1)
	{
        key_value.Key3_Press = 1;
		MY_BEEP_ON(50);

		ANO_DT_SendString(Str_RED,"KEY3 OK!");
	}
	press_flag[2] = key;
/*------------------------------*/
	key = KEY4_VALUE;
	if((key == 1 && !press_flag[3]) || MY_HMI.HMI_key[3] == 1)
	{
        key_value.Key4_Press = 1;
		MY_BEEP_ON(50);

		ANO_DT_SendString(Str_RED,"KEY4 OK!");
	}
	press_flag[3] = key;
/*------------------------------*/
	if(MY_HMI.HMI_key[4] != 0)
	{
		key_value.Key5_Press = MY_HMI.HMI_key[4];
		MY_BEEP_ON(50);

		ANO_DT_SendString(Str_RED,"other KEY OK!");
	}
}

void KEY_updata(void)
{
	static u8 postion_cnt = 0;
	Key_Scan();
	if(key_value.Key1_Press == 1)
	{
		// postion_set_x[postion_cnt] = postion_now_x;
		// postion_set_y[postion_cnt] = postion_now_y;
		// postion_cnt++;

		// HMI_position_updata(7);
		key_value.Key1_Press = 0;
		MY_HMI.HMI_key[0] = 0;
	}
	if(key_value.Key2_Press == 1)
	{
		// MY_Flsah_write();//Ð´

		// HMI_position_updata(7);
		key_value.Key2_Press = 0;
		MY_HMI.HMI_key[1] = 0;
	}
	if(key_value.Key3_Press == 1)
	{
		// MY_Flash_read();//¶Á

		// HMI_position_updata(7);
		key_value.Key3_Press = 0;
		MY_HMI.HMI_key[2] = 0;
	}
	if(key_value.Key4_Press == 1)
	{		
		key_value.Key4_Press = 0;
		MY_HMI.HMI_key[3] = 0;
	}
	if(key_value.Key5_Press != 0)
	{
//		MY_HMI_print("key=%d",MY_HMI.HMI_key[4]);

		switch(key_value.Key5_Press)
		{
			case 66:
			{
				LAZER_1_ON;
				break;
			}
			case 67:
			{
				LAZER_1_OFF;
				break;
			}
		}

		key_value.Key5_Press = 0;
		MY_HMI.HMI_key[4] = 0;
	}
}
