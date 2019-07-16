/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Library includes. */
#include "stm32f10x_it.h"
//bsp includes
#include "user_usart_control.h"
//使用freertos的绝对延时函数
xTaskHandle USART_HEART_BEAT;//发送给串口的心跳信息
/* 保存上次时间，调用后系统会自动更新它*/
static portTickType PreviousWakeTime;
void usart_heart_beat(){
	/* 设置延时时间，把时间转化tick节拍数 */
 const portTickType TimeIncrement = pdMS_TO_TICKS(5000);
	/* 获取当前系统时间 */
 PreviousWakeTime = xTaskGetTickCount();
		while(1){
			printf("系统已运行%lu秒\n",PreviousWakeTime/1000);
			vTaskDelayUntil(&PreviousWakeTime,TimeIncrement); /* 延时5000 tick，这里是5000ms */
		}
}
