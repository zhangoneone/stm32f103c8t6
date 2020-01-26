#include "user_kernel_control.hpp"
#include "user_timer.h"
#include "user_app_common.h"
#include "xprintf.h"
#include<vector>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<string>
using namespace std;
volatile uint32_t CPU_RunTime;
extern const sys_base_event_t sys_init_ok;
static EventBits_t r_event;//事件返回值
xTaskHandle Kernel_TASK_PCB;//Kernel线程句柄
Kernel::Kernel(){
	TIM5_Int_Init(9,719);//0.1ms中断1次
}
Kernel::~Kernel(){
	
}
unsigned char CPU_RunInfo[400];
void Kernel::top_command(){
	memset(CPU_RunInfo,0,400);
	vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息
	xprintf_s("---------------------------------------------\r\n");
	xprintf_s("任务名 任务状态 优先级 剩余栈 任务序号\r\n");
	xprintf_s("%s", CPU_RunInfo);
	xprintf_s("---------------------------------------------\r\n");

	memset(CPU_RunInfo,0,400); //信息缓冲区清零

	vTaskGetRunTimeStats((char *)&CPU_RunInfo);

	xprintf_s("任务名 运行计数 使用率\r\n");
	xprintf_s("%s", CPU_RunInfo);
	xprintf_s("---------------------------------------------\r\n\n");
//	vTaskDelay(1000); /* 延时1000 个 tick */
}
//友元函数，由c调用，本函数代理处理kernel类
void kernel_test_start(){
	 unsigned int save_value=0;
	//等待事件
		r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											sys_init_ok,//等待的事件
											pdFALSE,//true等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间	
		if(r_event & sys_init_ok == sys_init_ok){//sys_init_ok完成了
			Kernel *kernel=new Kernel();
			for(;;){
				xTaskNotifyWait(top_command_value,top_command_value,&save_value, portMAX_DELAY);
				if(save_value==top_command_value){
					save_value=0;
					kernel->top_command();
				}
			}
		}
}
