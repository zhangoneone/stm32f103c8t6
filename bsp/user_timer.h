#ifndef USER_TIMER_H
#define USER_TIMER_H
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
typedef enum{
	immediately=0,
}Update_option_t;

#endif