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
#include "user_crc.h"
#include "user_crc_control.h"
//这里使用freertos的信号量机制\
一个任务每隔3s释放一个二值信号量\
计算任务获取二值信号量后，计算crc，然后打印
//生成多项式0x4C11DB7 \
X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 + X8 + X7 + X4 + X2 + X +1
xTaskHandle CRC_CAL;//CRC计算
xTaskHandle CRC_POST_BINARY_SEM;
unsigned int crc_data[] = {0x0};//待求crc的数据
SemaphoreHandle_t crc_sem = NULL;
unsigned int crc_ret = 0;
extern SemaphoreHandle_t serial_sem;
void crc_cal(){
	//创建的二值信号量初始状态为空，必须先释放
	crc_sem = xSemaphoreCreateBinary();
	while(1){
		//先尝试获取信号量,最多等待2000tick
		if(xSemaphoreTake(crc_sem,2000) == pdTRUE ){
			crc_ret = user_cal_crc_32(crc_data[0]);
			//获取串口互斥量
			while(xSemaphoreTake(serial_sem,10) != pdTRUE );
			printf("数据0x%x的CRC校验结果是:0x%x",crc_data[0],crc_ret);
			//post
			xSemaphoreGive(serial_sem);
			vTaskDelay(1500); /* 延时1500 tick，这里是1500ms */
		}
	}
}

void crc_post_binary_sem(){
	while(1){
		vTaskDelay(portMAX_DELAY); /* 延时portMAX_DELAY tick */
		xSemaphoreGive(crc_sem);
	}
}

