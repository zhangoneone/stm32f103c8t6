#ifndef USER_FSMC_H
#define USER_FSMC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "user_common.h"
#include "stm32f10x.h"
#define FSMC	1
#if (FSMC ==1)
void FSMC_SRAM_Init(void);

#endif

#ifdef __cplusplus
}
#endif
#endif