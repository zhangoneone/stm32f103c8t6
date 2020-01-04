#ifndef USER_APP_COMMON_H
#define USER_APP_COMMON_H
/*@brief
存放APP需要用的全局OS定义
**********/

#include "user_common.h"
#include "user_led_control.h"
#include "user_usart_control.h"
#include "user_pwr_control.h"
#include "user_crc_control.h"
#include "user_rcc_control.h"
#include "user_flash_control.h"
#include "user_fs_control.h"
#include "user_io_control.h"
#include "user_shell_control.h"
#include "user_exti_control.h"
#include "user_timer_control.h"
#include "user_device_control.h"
typedef enum{
	SYS_INIT_OK = (0x01<<0),
	FLASH_INIT_OK = (0x01<<1),
	FS_MOUNT_OK = (0x01<<2),
	FS_FILE_OPERATE_OK = (0x01<<3),
	IO_OPERATE_OK = (0x01<<4),
}sys_base_event_t;//枚举类型


//以C写的函数，如果要被C++调用，需要以extern  “C”的方式，在其hpp文件中声明
//C函数列表
void xprintf_s(const char*	fmt,...);


//以C++写的函数，不应该直接被C直接包含，只能以extern的方式，放到common头文件中。
//C++函数列表
extern void do_action();

#endif