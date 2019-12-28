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
}Usart_Obj_st;

#ifdef USART1
#define USART1_TXB	128
#define USART1_RXB	128
extern const volatile Usart_Obj_st usart1_obj;
#endif
#ifdef USART2
#define USART2_TXB	128
#define USART2_RXB	128
extern const volatile Usart_Obj_st usart2_obj;
#endif
#ifdef USART3
#define USART3_TXB	128
#define USART3_RXB	128
extern const volatile Usart_Obj_st usart3_obj;
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
