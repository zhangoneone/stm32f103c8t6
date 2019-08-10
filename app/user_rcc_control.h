#ifndef USER_RCC_CONTROL_H
#define USER_RCC_CONTROL_H

void show_sys_clock();
typedef enum{
	SYS_INIT_OK = (0x01<<0),
	DEBUG_SERIAL_IDLE = (0x01<<1),
}sys_base_event_t;//枚举类型

extern xTaskHandle RCC_SYS_CLOCK;//打印系统时钟线程句柄
extern EventGroupHandle_t sys_base_event_group;//基本事件组
extern sys_base_event_t sys_init_ok;//枚举
extern sys_base_event_t debug_serial_idle;//枚举

#endif