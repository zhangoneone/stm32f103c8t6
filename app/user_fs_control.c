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
#include "user_fs_control.h"
#include "ff.h"
#include "user_app_common.h"
extern const sys_base_event_t fs_mount_ok;
extern const sys_base_event_t flash_init_ok;
static EventBits_t r_event;//事件返回值
FATFS *fs,fatfs;//fs操作句柄
#define work_buff_len 	512
void *work=NULL;
int clust_size = 0;
extern SemaphoreHandle_t serial_sem;
//fatfs文件系统使用:1)mount	\
2) mkfs	\
3)文件操作
//我们想让fs运行前，先完成裸板的flash测试\
为此，我们等到了flash测试完成事件，才会执行fs_init的相关工作。
void fs_init(){
		//等待事件
		r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											flash_init_ok,//等待的事件
											pdFALSE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间	
		if(r_event & flash_init_ok == flash_init_ok){//flash测试完成了
			work = pvPortMalloc(work_buff_len);//fs 格式化过程中的临时缓冲区
			fs = &fatfs;
			r_event = f_mkfs("0:",FM_FAT,1024,work,work_buff_len);
			//r_event = f_mkfs("",FM_SFD,0,work,work_buff_len);
			configASSERT(!r_event);
			r_event = f_mount(fs,"0:",1);
			configASSERT(!r_event);
			r_event = f_getfree("0:",&clust_size,&fs);
			configASSERT(!r_event);
			while(xSemaphoreTake(serial_sem,10) != pdTRUE );
			printf("fs mount success,size:%d clust\n",clust_size);
			xSemaphoreGive(serial_sem);
		}
		//fs初始化完成，事件置位
		xEventGroupSetBits(sys_base_event_group,fs_mount_ok);
}
xTaskHandle FS_TEST_TASK_PCB;//打印系统FLASH线程句柄
void fs_test(){
	  FIL fp;
	  char buffer[20];
	  int read_count=0;
  	fs_init();
	//等待事件
		r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											fs_mount_ok,//等待的事件
											pdFALSE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间	
		if(r_event & fs_mount_ok == fs_mount_ok){//fs已经挂载
			//r_event = f_open(&fp,"0:test.txt",FA_CREATE_NEW|FA_WRITE);
			//configASSERT(!r_event);
			//printf("    %d   ",r_event);
			//f_puts("hello world",&fp);
			//f_sync(&fp);
			//f_lseek(&fp,0);
			//f_read(&fp,buffer,20,&read_count);
			//f_close(&fp);
			while(xSemaphoreTake(serial_sem,10) != pdTRUE );
			printf("read text:%s\n",buffer);
			xSemaphoreGive(serial_sem);
			//任务完成，删除任务
			vTaskDelete(FS_TEST_TASK_PCB);
		}
}