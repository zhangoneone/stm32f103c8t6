#ifndef USER_PWR_H
#define USER_PWR_H
#include "user_common.h"
#include "stm32f10x.h"
#define PWR_MODE_MAX	2
typedef enum{
	//user_pwr_sleep = 0,
	user_pwr_standby = 0,
	user_pwr__stop,
}user_pwr_mode;
extern uchar user_pwr_mode_matrix[PWR_MODE_MAX];//标志stm32当前状态的，某个置1则在该模式下
//使能或者使能rtc和备份寄存器的访问
USER_RETURN_VALUE user_set_rtc_backup(FunctionalState NewState);
//睡眠模式
//USER_RETURN_VALUE user_sleep_mode();
//待机模式
USER_RETURN_VALUE user_standby_mode();
//停机模式
USER_RETURN_VALUE user_stop_mode();
void reboot();
//获取当前的电源管理状态
USER_RETURN_VALUE user_get_current_status(uchar *matrix);

#endif