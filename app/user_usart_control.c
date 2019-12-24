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
#include "user_usart_control.h"
#include "user_rcc_control.h"
#include "user_app_common.h"

//使用freertos的绝对延时函数
xTaskHandle USART_HEART_BEAT;//发送给串口的心跳信息
SemaphoreHandle_t serial_sem = NULL;//串口空闲互斥量
/* 保存上次时间，调用后系统会自动更新它*/
static portTickType PreviousWakeTime;
static EventBits_t r_event;//事件返回值
void usart_heart_beat(){
	/* 设置延时时间，把时间转化tick节拍数 */
	const portTickType TimeIncrement = pdMS_TO_TICKS(5000);
	/* 获取当前系统时间 */
  PreviousWakeTime = xTaskGetTickCount();
	while(1){
			vTaskDelayUntil(&PreviousWakeTime,TimeIncrement); /* 延时5000 tick，这里是5000ms */
			//wait 循环等待，每次等不到则task休眠10tick。
			while(xSemaphoreTake(serial_sem,10) != pdTRUE );
			printf("running time:%lus\n",PreviousWakeTime/1000);
			//post
			xSemaphoreGive(serial_sem);
	}
}
