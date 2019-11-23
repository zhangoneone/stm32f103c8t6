#ifndef USER_CRC_H
#define USER_CRC_H
#include "user_common.h"
void user_crc_init();
uint32 user_cal_crc_32(uint32 data);
uint32 user_cal_crc(uint32 *ptr,uint32 len);
#endif