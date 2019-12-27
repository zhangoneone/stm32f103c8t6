/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "user_io_control.h"
#include "xprintf.h"
#include "user_app_common.h"
xTaskHandle IO_TASK_PCB;//IO线程句柄
SemaphoreHandle_t serial_sem = NULL;//串口空闲互斥量
extern const sys_base_event_t fs_file_operate_ok;
extern const sys_base_event_t io_operate_ok;
static EventBits_t r_event;//事件返回值
static char buff[20];
void io_out(){
	xprintf_s("io test success\n");
	xprintf_s("中文输出成功\n");
}
void io_in(){
	xprintf_s("enter any continue...\n");
	xgets(buff,20);
}

void io_test(){
		//等待事件
		r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											fs_file_operate_ok,//等待的事件
											pdFALSE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间	
		if(r_event & fs_file_operate_ok == fs_file_operate_ok){//fs已经挂载
			io_out();
			io_in();
			//事件置位
			xEventGroupSetBits(sys_base_event_group,io_operate_ok);
			vTaskDelete(NULL);
		}
}