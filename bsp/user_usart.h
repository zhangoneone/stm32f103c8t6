#ifndef USERUSARTH
#define USERUSARTH
#ifdef __cplusplus
extern "C" {
#endif
#include "user_common.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
typedef struct{
  int (*u_putc)(uchar c);
	uchar (*u_getc)();
	uchar* (*u_gets)(USART_TypeDef * Usart_Num);
	int (*u_puts)(USART_TypeDef * Usart_Num,uchar *s);
}usart_obj;

#ifdef USART1
#define USART1_TXB	128
#define USART1_RXB	128
extern const volatile usart_obj usart1_obj;
#endif
void USART1_Config(int boaud_rate);
void USART2_Config(int boaud_rate);
void USART3_Config(int boaud_rate);
void USART4_Config(int boaud_rate);
void USART5_Config(int boaud_rate);
void user_Usart_SendByte(USART_TypeDef * Usart_Num,uchar SendData);
//返回写入的字节数,不含字符串结束符。没有溢出检查
int user_Usart_Send(USART_TypeDef * Usart_Num,const uchar *str);
#ifdef __cplusplus
}
#endif
#endif
