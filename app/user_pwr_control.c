/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* Library includes. */
#include "stm32f10x_it.h"
//bsp includes
#include "user_pwr.h"
#include "user_pwr_control.h"
xTaskHandle PWR_MODE_SWITCH;//电源模式切换
xTaskHandle PWR_MODE_SEND_MSG;//电源模式发送消息
//这里使用消息队列，获取消息才进入待机模式
QueueHandle_t IntMsg_Q = NULL;
void pwr_mode_switch(){
	  int rcv_Q = -1;
	  IntMsg_Q = xQueueCreate(IntMsg_Q_MAX_LEN,sizeof( int ));
		while(1){
			xQueueReceive(IntMsg_Q,&rcv_Q,portMAX_DELAY);//无限等待
			xprintf_s("收到%d",rcv_Q);
			if(rcv_Q == enter_standby_msg){
				xprintf_s("即将进入待机模式...\n");
				user_standby_mode();
		}
	}
}

void pwr_mode_send_msg(){
	  int msg = -1;
		while(1){
			  vTaskDelay(portMAX_DELAY);//延时portMAX_DELAY个时基，无限延时
				xprintf_s("发送待机消息...\n");
				msg = enter_standby_msg;
				if(xQueueSend(IntMsg_Q,(void *)&msg,0) == pdPASS)
					vTaskDelete(NULL);//发送完成，删除自己
				else{
					xprintf_s("消息发送失败...\n");
				}
		}
}


