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
static EventBits_t r_event;//ÊÂ¼ş·µ»ØÖµ
FATFS *fs,fatfs;//fs²Ù×÷¾ä±ú
#define work_buff_len 	512
void *work=NULL;
int clust_size = 0;
extern SemaphoreHandle_t serial_sem;
//fatfsÎÄ¼şÏµÍ³Ê¹ÓÃ:1)mount	\
2) mkfs	\
3)ÎÄ¼ş²Ù×÷
//ÎÒÃÇÏëÈÃfsÔËĞĞÇ°£¬ÏÈÍê³ÉÂã°åµÄflash²âÊÔ\
Îª´Ë£¬ÎÒÃÇµÈµ½ÁËflash²âÊÔÍê³ÉÊÂ¼ş£¬²Å»áÖ´ĞĞfs_initµÄÏà¹Ø¹¤×÷¡
void fs_init(){
		//µÈ´ıÊÂ¼ş
		r_event = xEventGroupWaitBits(sys_base_event_group,//ÊÂ¼ş×é¾ä±ú
											flash_init_ok,//µÈ´ıµÄÊÂ¼ş
											pdFALSE,//true µÈµ½ºóÇå³ıÊÂ¼şÎ»
											pdTRUE,//trueÂß¼­ÓëµÈ´ı
											portMAX_DELAY);//µÈ´ıÊ±¼ä	
		if(r_event & flash_init_ok == flash_init_ok){//flash²âÊÔÍê³ÉÁË
			configASSERT((clust_size = xPortGetFreeHeapSize())>=work_buff_len);
			printf("      %d       ",clust_size);
			work = pvPortMalloc(work_buff_len);//fs ¸ñÊ½»¯¹ı³ÌÖĞµÄÁÙÊ±»º³åÇø
			fs = &fatfs;
			r_event = f_mkfs("1:", 0, work, work_buff_len);
			configASSERT(!r_event);
			printf("      %d       ",r_event);
			r_event = f_mount(fs, "1:", 1);
			configASSERT(!r_event);
			r_event = f_getfree("1:",&clust_size,&fs);
			configASSERT(!r_event);
			while(xSemaphoreTake(serial_sem,10) != pdTRUE );
			printf("fs mount success,size:%d clust\n",clust_size);
			xSemaphoreGive(serial_sem);
		}
		//fs³õÊ¼»¯Íê³É£¬ÊÂ¼şÖÃÎ»
		xEventGroupSetBits(sys_base_event_group,fs_mount_ok);
}
xTaskHandle FS_TEST_TASK_PCB;//´òÓ¡ÏµÍ³FLASHÏß³Ì¾ä±ú

/***attention!!!!**/

/*****
freertosµÄÈÎÎñ¶ÑÕ»ºÜĞ¡£¬ËùÒÔ¾¡Á¿²»ÒªÔÚÈÎÎñ¶ÑÕ»ÀïÃæ¶¨Òå½Ï´ó±äÁ¿£¬ÒÔÃâÈÎÎñ¶ÑÕ»Òç³ö£¡£¡£¡
*********/
FIL fp;
char buffer[20];
int count=0;
void fs_test(){
  	fs_init();
	//µÈ´ıÊÂ¼ş
		r_event = xEventGroupWaitBits(sys_base_event_group,//ÊÂ¼ş×é¾ä±ú
											fs_mount_ok,//µÈ´ıµÄÊÂ¼ş
											pdFALSE,//true µÈµ½ºóÇå³ıÊÂ¼şÎ»
											pdTRUE,//trueÂß¼­ÓëµÈ´ı
											portMAX_DELAY);//µÈ´ıÊ±¼ä	
		if(r_event & fs_mount_ok == fs_mount_ok){//fsÒÑ¾­¹ÒÔØ
			r_event = f_open(&fp, "1:hello.txt", FA_CREATE_NEW | FA_WRITE);
			configASSERT(!r_event);
			f_write(&fp, "Hello, World!\r\n", 15, &count);
			configASSERT(count == 15);
			f_close(&fp);
			r_event = f_open(&fp, "1:hello.txt", FA_READ);
			configASSERT(!r_event);
			f_read(&fp,buffer,15,&count);
			f_close(&fp);
			while(xSemaphoreTake(serial_sem,10) != pdTRUE );
			printf("read text:%s\n",buffer);
			xSemaphoreGive(serial_sem);
			vPortFree(work);
			f_unmount("1:");
			while(1);
			//ÈÎÎñÍê³É£¬É¾³ıÈÎÎñ  É¾³ı×ÔÉí£¬²ÎÊıÌîNULL  ÔÚÕâÀïÉ¾³ı×Ô¼º»á³ö´í£¬ÎªÊ²Ã´£¿£¿
			//vTaskDelete(NULL);
		}
}