#ifndef USER_LED_H
#define USER_LED_H
#include "user_common.h"
//这个最小系统的测试LED接的是VCC和PC13，所以PC13配置为输出低电平即可点亮LED
#define LED_PORT		(GPIOC)
#define LED_PIN			(GPIO_Pin_13)
#define LED_RCC_ENABLE()	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE)

void user_led_init();
void user_led_on();
void user_led_off();

#endif