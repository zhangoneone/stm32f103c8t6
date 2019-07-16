//#include "user_app_config.h"
//freertos kernel head file
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//freertos app head file
#include "user_led_control.h"
#include "user_usart_control.h"
#include "user_pwr_control.h"
//添加app,创建任务						
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
	xTaskCreate(	usart_heart_beat,
							"usart_heart_beat_task",	
							128,
							NULL,
							3,
							&USART_HEART_BEAT );
	xTaskCreate(	pwr_mode_switch,
							"pwr_mode_switch_task",	
							128,
							NULL,
							3,
							&PWR_MODE_SWITCH );
	xTaskCreate(	pwr_mode_send_msg,
							"pwr_mode_send_msg_task",	
							128,
							NULL,
							3,
							&PWR_MODE_SEND_MSG );
	return 0;
}