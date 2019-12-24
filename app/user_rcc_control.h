#ifndef USER_RCC_CONTROL_H
#define USER_RCC_CONTROL_H

void show_sys_clock();

extern xTaskHandle RCC_SYS_CLOCK;//打印系统时钟线程句柄
extern EventGroupHandle_t sys_base_event_group;//基本事件组


#endif