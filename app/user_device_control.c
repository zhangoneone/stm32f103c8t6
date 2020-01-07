/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "user_device_control.h"
//包含所有外部设备的测试
void eeprom_test();
void w25qxx_test();
void net_init_done_callback(void *args);
xTaskHandle Device_Test_Task_TCB;
void device_test(){
	eeprom_test();
	w25qxx_test();
	tcpip_init(net_init_done_callback,0);
	vTaskDelete(NULL);//任务完成，删除任务
}