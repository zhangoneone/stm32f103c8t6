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
#include "user_rcc_control.h"
#include "user_rcc.h"
//使用freertos的事件。\
等待到系统启动完成事件后，打印输出系统当前的时钟\
为了避免多个线程争用打印串口，使用等待串口空闲的事件

EventGroupHandle_t sys_base_event_group;
xTaskHandle RCC_SYS_CLOCK;//打印系统时钟线程句柄

sys_base_event_t sys_init_ok = SYS_INIT_OK;//枚举
sys_base_event_t debug_serial_idle = DEBUG_SERIAL_IDLE;//枚举

static unsigned int user_sys_clock = 0;
static EventBits_t r_event;//事件返回值
//xEventGroupClearBits() 清除事件位
//
void show_sys_clock(){
	while(1){
		user_sys_clock = user_Get_Sys_Clock();
		//等待事件
		r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											sys_init_ok|debug_serial_idle,//等待的事件
											pdTRUE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间
		
		if(r_event & (sys_init_ok|debug_serial_idle) 
										== (sys_init_ok|debug_serial_idle)){
											
			printf("当前系统时钟:%uMHZ",user_sys_clock/1000000);
			//标志串口空闲事件
			xEventGroupSetBits(sys_base_event_group,sys_init_ok|debug_serial_idle);
											
			//!!!!!attention!!!!!!
			/*上面的语句，因为缺少了sys_init_ok| 导致事件整体工作不正常。
			使用时应该申请了多少事件，就设置多少事件，
		  具体的原因还未知，等进一步探索之后回来再补充	*/

			vTaskDelete(RCC_SYS_CLOCK);//任务完成，删除任务
		}
	}
}
