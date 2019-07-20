#ifndef USER_CRC_CONTROL_H
#define USER_CRC_CONTROL_H
extern xTaskHandle CRC_CAL;//CRCº∆À„
extern xTaskHandle CRC_POST_BINARY_SEM;
extern void crc_cal();
extern void crc_post_binary_sem();
#endif