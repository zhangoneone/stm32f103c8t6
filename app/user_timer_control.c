
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
	vTaskDelete(NULL);
}