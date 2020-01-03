#ifndef USER_WATCHDOG_H
#define USER_WATCHDOG_H
#ifdef __cplusplus
extern "C" {
#endif
#include "user_common.h"
#include "stm32f10x.h"
#define IWDG	1
#define WWDG	1
#if (IWDG ==1)
void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Start();
void IWDG_Feed(void);
void IWDG_Init_to_1s(); 	
#endif

#if (WWDG ==1)
void WWDG_Init(u8 tr,u8 wr,u32 fprer);
//WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);//计数器值为7f,窗口寄存器为5f,分频数为8
void WWDG_Start();
void WWDG_Set_Counter(u8 cnt);//range,0x40~0x7f
#endif
#ifdef __cplusplus
}
#endif
#endif