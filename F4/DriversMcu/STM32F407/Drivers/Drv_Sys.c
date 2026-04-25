#include "Drv_Sys.h"
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h"

static uint64_t SysRunTimeMs = 0;
extern uint64_t SysRunTimeMs;

/*初始化滴答计时器*/
void SysTick_Init(void )
{
	RCC_ClocksTypeDef  rcc_clocks;	//系统时钟结构体
    uint32_t         cnts;			//计数

    RCC_GetClocksFreq ( &rcc_clocks );

    cnts = ( uint32_t ) rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;
    cnts = cnts / 8;

    SysTick_Config ( cnts );
    SysTick_CLKSourceConfig ( SysTick_CLKSource_HCLK_Div8 );
}
/*滴答计时器中断*/
void SysTick_Handler(void)//
{
	SysRunTimeMs++;
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}
/*获取当前系统毫秒*/
uint32_t GetSysRunTimeMs(void)
{
	return SysRunTimeMs;
}
/*获取当前系统微秒*/
uint32_t GetSysRunTimeUs(void)
{
	register uint32_t ms;
    u32 value;
	do
	{
    ms = SysRunTimeMs;
    value = ms * TICK_US + ( SysTick->LOAD - SysTick->VAL ) * TICK_US / SysTick->LOAD;
	}
	while(ms != SysRunTimeMs);
	return value;
}
/*系统延时微秒*/
/*微秒*/
void MyDelayUs ( uint32_t us )
{
    uint32_t now = GetSysRunTimeUs();
    while ( GetSysRunTimeUs() - now < us );
}
/*系统延时毫秒*/
/*毫秒*/
void MyDelayMs(u32 time)
{
	while ( time-- )
        MyDelayUs ( 1000 );
}

void DrvSysInit(void)
{
	//中断优先级组别设置
	NVIC_PriorityGroupConfig(NVIC_GROUP);		
	//滴答时钟
	SysTick_Init(); 		
}

