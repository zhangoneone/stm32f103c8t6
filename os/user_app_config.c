//#include "user_app_config.h"
//freertos kernel head file
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//freertos app head file
#include "user_led_control.h"


static  xTaskHandle OPEN_LED_Task_TCB;
static  xTaskHandle CLOSE_LED_Task_TCB;
//Ìí¼Óapp							
int freertos_app_add(){
	xTaskCreate(	open_led,
							"open_led_task",	
							128,
							NULL,
							3,
							&OPEN_LED_Task_TCB );
	xTaskCreate(	close_led,
							"close_led_task",	
							128,
							NULL,
							3,
							&CLOSE_LED_Task_TCB );
	return 0;
}