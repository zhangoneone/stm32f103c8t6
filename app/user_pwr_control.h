#ifndef USEER_PWR_CONTROL_H
#define USEER_PWR_CONTROL_H
extern xTaskHandle PWR_MODE_SWITCH;//电源模式切换
extern xTaskHandle PWR_MODE_SEND_MSG;//电源模式发送消息
typedef enum{
	 enter_standby_msg = 0,
}IntMsg;//整型消息
#define IntMsg_Q_MAX_LEN	10
extern QueueHandle_t IntMsg_Q;
void pwr_mode_switch();
void pwr_mode_send_msg();

//总结，注意消息类型一定要保持一致，否则内核会assert，并且会得到不对的数据。\
注意无符号数和有符号数之间的区别
#endif