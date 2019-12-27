/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
/*user Library*/
#include "user_shell_control.h"
#include "nr_micro_shell.h"
#include "xprintf.h"
#include "user_app_common.h"
xTaskHandle Shell_Test_Task_TCB;
extern const sys_base_event_t io_operate_ok;
extern void do_action();
static EventBits_t r_event;//事件返回值
//定期扫描，如果发现新字符则传递给shell
//由于shell输入是低速的，所以每次发现新字符并且输入后，可以稍等片刻再扫描
void shell_test(){
			//等待事件
		r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											io_operate_ok,//等待的事件
											pdFALSE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间	
		if(r_event & io_operate_ok == io_operate_ok){//io已经就绪
			shell_init();
			do_action();
			while(1){
				//字符传递给shell
				shell(xgetc());
				//休眠片刻
				vTaskDelay(50);//休眠50ms
    }
	}
}