#ifndef USER_TIMER_H
#define USER_TIMER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "user_common.h"
#include "stm32f10x.h"

/*@brief
user_timer应该提供软件延时功能和PWM等功能，timer作为一个驱动的底层支持
有关于时间的都应该在这里完成。
不过也要注意到freertos已经提供了一些软件定时器。但是freertos的定时器都是基于
systick，时间精度是以tick为单位的。systick定时器一般AHB时钟8分频，即9MHZ。
systick的初始校准值是9000。因此产生1ms的时基

*/

/*brief
stm32的硬件定时器
TIM1和TIM8高级控制定时器
TIM6和TIM7基本定时器
TIM2~TIM5通用定时器
不是所有型号的stm32都支持以上的定时器的
*/

#if defined(TIM6)&&defined(TIM7)
typedef struct{
	void(*init)(u16 arr,u16 psc);
	void(*update)(TIM_TypeDef *TIMx,u16 arr,u16 psc);
	void(*start)(TIM_TypeDef *TIMx);
	void(*stop)(TIM_TypeDef *TIMx);
	void(*deinit)(TIM_TypeDef *TIMx);
}Basic_Timer_t;

extern const Basic_Timer_t tim6;
extern const Basic_Timer_t tim7;
extern volatile char tim6_int_flag;//每产生一次中断，值加1
extern volatile char tim7_int_flag;
#endif

#ifdef __cplusplus
}
#endif
#endif