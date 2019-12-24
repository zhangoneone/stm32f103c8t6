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
typedef enum{
	SYS_INIT_OK = (0x01<<0),
	FLASH_INIT_OK = (0x01<<1),
	FS_MOUNT_OK = (0x01<<2),
}sys_base_event_t;//枚举类型


#endif