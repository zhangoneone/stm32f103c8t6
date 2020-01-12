/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Library includes. */
#include "stm32f10x_it.h"
/*user Library*/
#include "user_led.h"
#include "user_led_control.h"
xTaskHandle OPEN_LED_Task_TCB;
xTaskHandle CLOSE_LED_Task_TCB;
//使用freertos的挂起任务api
void open_led(){
	while(1){
		user_led_on();
		vTaskSuspend(CLOSE_LED_Task_TCB);//开灯后挂起关灯任务
		vTaskDelay(500); /* 延时500 tick，这里是500ms */
		vTaskResume(CLOSE_LED_Task_TCB);//500ms后恢复关灯任务
	}
}

void close_led(){
	 while(1){
		user_led_off();
		vTaskSuspend(OPEN_LED_Task_TCB);//关灯后挂起开灯任务
		vTaskDelay(500); /* 延时500 tick，这里是500ms */
		vTaskResume(OPEN_LED_Task_TCB);//500ms后恢复开灯任务
		vTaskDelay(500);
		user_led_off();
		vTaskDelay(500);
	}
}