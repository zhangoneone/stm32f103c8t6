
/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
//user include
#include "user_timer_control.h"
#include "user_timer.h"
#include "user_app_common.h"
#include "user_led.h"

xTaskHandle Basic_Timer_Test_Task_TCB;
void basic_timer_test(){
	int i =6;
	tim6.init(7199,9999);//将会产生1s一次的中断
	tim7.init(7199,9999);//将会产生1s一次的中断
	tim6.start(TIM6);
	tim6.start(TIM7);
	while(i--){
		if(i==3){
			tim6.update(TIM6,7199,4999);//将会产生0.5s一次的中断
			tim7.update(TIM7,7199,4999);
		}
		xprintf_s("tim6:%d tim7:%d\r\n",tim6_int_flag,tim7_int_flag);
		vTaskDelay(1001);//休眠1001ms
	}
	vTaskDelete(NULL);
}