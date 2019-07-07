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


void hardware_init(){
	USART1_Config(115200);
	printf("stm32 setup!\n");
}
int main( void ){
	hardware_init();
	vTaskStartScheduler();
	return 0;
}
