#ifndef USER_LED_CONTROL_H
#define USER_LED_CONTROL_H
extern  xTaskHandle OPEN_LED_Task_TCB;
extern  xTaskHandle CLOSE_LED_Task_TCB;
void open_led();
void close_led();

#endif
