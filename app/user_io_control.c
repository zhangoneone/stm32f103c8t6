/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "xprintf.h"
#include "user_io_control.h"
xTaskHandle IO_TASK_PCB;//IOÏß³Ì¾ä±ú
SemaphoreHandle_t serial_sem = NULL;//´®¿Ú¿ÕÏÐ»¥³âÁ¿
static char buff[20];
void io_out(){
	xprintf_s("io out test\n");
}
void io_in(){
	xprintf_s("waiting for input...\n");
	xgets(buff,20);
}

void io_test(){
	while(1){
		io_out();
		io_in();
		vTaskDelete(NULL);
	}
}