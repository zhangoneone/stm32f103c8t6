/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
/* Library includes. */
#include "stm32f10x_it.h"
//bsp includes
#include "user_flash_control.h"
#include "user_flash.h"
#include "user_app_common.h"
extern EventGroupHandle_t sys_base_event_group;//引用外部的事件组
xTaskHandle FLASH_SIZE_TASK_PCB;//打印系统FLASH线程句柄
static EventBits_t r_event;//事件返回值
static unsigned short flash_size_value=0;

void flash_read_write_test(){
	char *buff = pvPortMalloc(1024);
	flash_driver.initialize(0);
	flash_driver.write(0,"test",0,1);
	flash_driver.read(0,buff,0,1);
	printf("%s",buff);
	vPortFree(buff);
}
void flash_test(){
	  flash_size_value = flash_size();
		while(1){
			//等待串口空闲事件
			r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											debug_serial_idle,//等待的事件
											pdTRUE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间
			if((r_event &debug_serial_idle)==debug_serial_idle){//检查等到的事件，是否为想要的事件
				printf("FLASH:%uKB\n",flash_size_value);
				//标志串口空闲事件(释放事件)
				xEventGroupSetBits(sys_base_event_group,debug_serial_idle);
				flash_read_write_test();
				//任务完成，删除任务
				vTaskDelete(FLASH_SIZE_TASK_PCB);
			}
		}
}

