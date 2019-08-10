#include "user_rcc.h"
#include "stm32f10x_rcc.h"
/*
rcc是复位和时钟控制 
stm32的复位分为系统复位 上电复位 备份区域复位
复位功能暂时不表

三种时钟源
HSI振荡器时钟
HSE振荡器时钟
PLL时钟

AHB 8分频后提供给Systick.Systick一般作为os的时基
RCC_CR寄存器指示了当前系统ready的时钟源，以及正在使用哪一个时钟源

PA8可以作为时钟源的输出引脚 MCO
*/
extern uint32_t SystemCoreClock;//系统时钟

uint32 user_Get_Sys_Clock(){
	return SystemCoreClock;
}
