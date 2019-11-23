#include "user_rcc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
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

//配置系统时钟
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量

	RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
	RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
	HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
	if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
		FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
			
		RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
		RCC_PCLK1Config(RCC_HCLK_Div2);                //配置APB1(PCLK1)钟==AHB1/2时钟
			 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
		RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
		while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
		{
		}
	}
}
