/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Library includes. */
#include "stm32f10x_it.h"
/*user Library*/
#include "user_led.h"
#include "user_led_control.h"
#include "user_timer.h"
xTaskHandle OPEN_LED_Task_TCB;
//使用freertos的挂起任务api
void open_led(){
	int i =0;
	TIM3_PWM_Config(10*1000,0);//初始频率是10khz，占空比为0
	while(1){
		for(i=1;i<=40;i++){
			TIM3_PWM_Set_Rate(0.01*i);//最终占空比是0.4
			vTaskDelay(20); /* 延时20tick，这里是20ms */
		}
		for(i=1;i<=40;i++){
			TIM3_PWM_Set_Rate(0.01*(40-i));//最终占空比是0
			vTaskDelay(20); /* 延时20tick，这里是20ms */
		}
	}
}