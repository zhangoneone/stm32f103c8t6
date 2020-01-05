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
extern const sys_base_event_t fs_file_operate_ok;
static EventBits_t r_event;//ÊÂ¼ş·µ»ØÖµ
FATFS *fs,fatfs;//fs²Ù×÷¾ä±ú
const int work_buff_len = 512;
char work[work_buff_len];
int clust_size = 0;
xTaskHandle FS_TEST_TASK_PCB;//´òÓ¡ÏµÍ³FLASHÏß³Ì¾ä±ú
/***attention!!!!**/
/*****
freertosµÄÈÎÎñ¶ÑÕ»ºÜĞ¡£¬ËùÒÔ¾¡Á¿²»ÒªÔÚÈÎÎñ¶ÑÕ»ÀïÃæ¶¨Òå½Ï´ó±äÁ¿£¬ÒÔÃâÈÎÎñ¶ÑÕ»Òç³ö£¡£¡£¡
*********/
static FIL fp;
char buffer[20];
int count=0;

//fatfsÎÄ¼şÏµÍ³Ê¹ÓÃ:1)mkfs	\
2) mount	\
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
			//¹ÒÔØÎÄ¼şÏµÍ³£¬Ã»ÓĞÔò´´½¨
//			fs = &fatfs;
//			r_event = f_mount(fs, "FLASH",1);
//			if(FR_NO_FILESYSTEM == r_event){
//				xprintf_s("not exist filesystem,create it on flash...\n");
//				r_event = f_mkfs("FLASH", 0, work, work_buff_len);
//				configASSERT(!r_event);
//				xprintf_s("   %d   ",r_event);
//				r_event = f_mount(fs, "FLASH",1);
//				configASSERT(!r_event);
//			}
//			//ÆäËû¹ÊÕÏ£¬assert
//			configASSERT(!r_event);
//			r_event = f_getfree("FLASH",&clust_size,&fs);
//			configASSERT(!r_event);
//			xprintf_s("fs mount success,size:%d clust\n",clust_size);
			
			//¹ÒÔØÎÄ¼şÏµÍ³£¬Ã»ÓĞÔò´´½¨
			fs = &fatfs;
			r_event = f_mount(fs, "OUT_FLASH",1);
			if(FR_NO_FILESYSTEM == r_event){
				xprintf_s("not exist filesystem,create it on flash...\n");
				r_event = f_mkfs("OUT_FLASH", 0, work, work_buff_len);
				configASSERT(!r_event);
				xprintf_s("   %d   ",r_event);
				r_event = f_mount(fs, "OUT_FLASH",1);
				configASSERT(!r_event);
			}
			//ÆäËû¹ÊÕÏ£¬assert
			configASSERT(!r_event);
			r_event = f_getfree("OUT_FLASH",&clust_size,&fs);
			configASSERT(!r_event);
			xprintf_s("fs mount success,size:%d clust\n",clust_size);
		}
		//fs³õÊ¼»¯Íê³É£¬ÊÂ¼şÖÃÎ»
		xEventGroupSetBits(sys_base_event_group,fs_mount_ok);
}

void fs_test(){
  	fs_init();
	//µÈ´ıÊÂ¼ş
		r_event = xEventGroupWaitBits(sys_base_event_group,//ÊÂ¼ş×é¾ä±ú
											fs_mount_ok,//µÈ´ıµÄÊÂ¼ş
											pdFALSE,//true µÈµ½ºóÇå³ıÊÂ¼şÎ»
											pdTRUE,//trueÂß¼­ÓëµÈ´ı
											portMAX_DELAY);//µÈ´ıÊ±¼ä	
		if(r_event & fs_mount_ok == fs_mount_ok){//fsÒÑ¾­¹ÒÔØ
//			r_event = f_open(&fp, "FLASH:hello.txt", FA_CREATE_NEW | FA_WRITE);
//			configASSERT(!r_event);
//			f_write(&fp, "Hello, World!\nÄãºÃÊÀ½ç", 30, &count);
//			configASSERT(count == 30);
//			f_close(&fp);
//			r_event = f_open(&fp, "FLASH:hello.txt", FA_READ);
//			configASSERT(!r_event);
//			f_read(&fp,buffer,30,&count);
//			f_close(&fp);
//			xprintf_s("read text:%s\n",buffer);
			//fs»ù±¾¶ÁĞ´Íê³É£¬ÊÂ¼şÖÃÎ»
			xEventGroupSetBits(sys_base_event_group,fs_file_operate_ok);
			//ÈÎÎñÍê³É£¬É¾³ıÈÎÎñ  É¾³ı×ÔÉí£¬²ÎÊıÌîNULL
			vTaskDelete(NULL);
		}
}