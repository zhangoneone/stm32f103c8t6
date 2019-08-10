//#include "user_app_config.h"
//freertos kernel head file
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
//freertos app head file
#include "user_app_common.h"
int freertos_app_add();

//初始化参数和内核参数，并且添加app
int software_init(){
	//创建事件组
	sys_base_event_group = xEventGroupCreate();
	//将两个事件置位
	xEventGroupSetBits(sys_base_event_group,sys_init_ok|debug_serial_idle);
	freertos_app_add();
}
//添加app,创建任务						
int freertos_app_add(){
	xTaskCreate(	show_sys_clock,
							"show_sys_clock_task",	
							128,
							NULL,
							3,
							&RCC_SYS_CLOCK );
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
	xTaskCreate(	crc_cal,
							"crc_cal_task",	
							128,
							NULL,
							3,
							&CRC_CAL );
  xTaskCreate(	crc_post_binary_sem,
							"crc_post_binary_sem_task",	
							128,
							NULL,
							3,
							&CRC_POST_BINARY_SEM );
	return 0;
}