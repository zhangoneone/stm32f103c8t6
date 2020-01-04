/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "user_device_control.h"
//包含所有外部设备的测试
void eeprom_test();
xTaskHandle Device_Test_Task_TCB;
void device_test(){
	eeprom_test();
	vTaskDelete(NULL);//任务完成，删除任务
}