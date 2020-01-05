#ifndef USER_SPI_H
#define USER_SPI_H
#ifdef __cplusplus
extern "C" {
#endif
/*@brief
IO口有GPIO和AFIO功能。不准备单独编写IO的程序。
编写软件模拟i2c和spi，完成IO的使用
**/
#include "user_common.h"
void SPI1_Init(void);
void SPI2_Init(void);	
u8 SPI1_ReadWriteByte(u8 TxData);
u8 SPI2_ReadWriteByte(u8 TxData);
void SPI1_SetSpeed(u8 SpeedSet);
void SPI2_SetSpeed(u8 SpeedSet);
#ifdef __cplusplus
}
#endif
#endif