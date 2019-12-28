#ifndef USER_LED_H
#define USER_LED_H
#include "user_common.h"

#define LED_RCC(T)	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO##T , ENABLE)

#if C8T6==1
//这个最小系统的测试LED接的是VCC和PC13，所以PC13配置为输出低电平即可点亮LED
#define LED_PORT		(GPIOC)
#define LED_PIN			(GPIO_Pin_13)
#define LED_RCC_ENABLE() LED_RCC(C)

#elif ZET6==1
//这个是战舰开发板的LED定义LED0接了PB5,LED1接了PE5
#define LED_PORT		(GPIOB)
#define LED_PIN			(GPIO_Pin_5)

#define LED_PORT1		(GPIOE)
#define LED_PIN1			(GPIO_Pin_5)
#define LED_RCC_ENABLE() LED_RCC(B);\
																		LED_RCC(E)
#else
#error please select product type
#endif
void user_led_init();
void user_led_on();
void user_led_off();
#if ZET6==1
void user_led1_on();
void user_led1_off();
#endif
#endif