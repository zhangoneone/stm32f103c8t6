
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
extern volatile u8  TIM5CH1_CAPTURE_STA;	//输入捕获状态		    				
extern volatile u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
xTaskHandle Basic_Timer_Test_Task_TCB;

void show_capture(){
	u32 temp=0;
	if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			xprintf_s("HIGH:%d us\r\n",temp);//打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
}

void basic_timer_test(){
	int i =10;
	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数
	tim6.init(7199,59999);//将会产生6s一次的中断
	tim7.init(7199,59999);//将会产生6s一次的中断
	tim6.start(TIM6);
	tim6.start(TIM7);
	while(i--){
		if(i==5){
			tim6.update(TIM6,7199,9999,immediately);//将会产生1s一次的中断
			tim7.update(TIM7,7199,9999,immediately);
		}
		xprintf_s("tim6:%d tim7:%d\r\n",tim6_int_flag,tim7_int_flag);
		vTaskDelay(1001);//休眠1001ms
	}
	while(1){
		show_capture();
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
}