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
static void hardware_init(){
	USART1_Config(115200);
	user_led_init();
	user_crc_init();
	printf("bsp setup!");
}

int main( void ){
	hardware_init();//初始化硬件平台
	software_init();//初始化os和app
	vTaskStartScheduler();//开始调度
	return 0;
}
