/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Library includes. */
#include "stm32f10x_it.h"
/*user Library*/
#include "user_usart.h"
#include "user_led.h"
#include "user_crc.h"
#include "user_dma.h"
#include "user_timer.h"
static void hardware_init(){
  //因为还没有加载os，不能使用xprintf_s，使用以下函数，指定out流
	extern void xfprintf(void(*func)(unsigned char),	const char*	fmt,...);
	USART1_Config(115200);
	USART2_Config(115200);
	USART3_Config(115200);
	user_led_init();
	user_crc_init();
	xfprintf(usart1_obj.u_putc,"bsp setup!\r\n");
}

int main( void ){
	hardware_init();//初始化硬件平台
	software_init();//初始化os和app
	vTaskStartScheduler();//开始调度
	return 0;
}
