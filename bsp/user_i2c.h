#ifndef USER_I2C_H
#define USER_I2C_H
#ifdef __cplusplus
extern "C" {
#endif
/*@brief
IO口有GPIO和AFIO功能。不准备单独编写IO的程序。
编写软件模拟i2c和spi，完成IO的使用
**/

/*@brief
i2c的延时一般设置为1us，这个时间比os使用的1ms时基要小的多
使用while循环产生的时间延时不会很精准。
因此使用定时器产生计数脉冲，然后while轮询。
这样虽然系统会损失1us的性能，但是比频繁的中断处理要好的多

另外由于os参与调度。所以延时期间应该屏蔽中断，以免延时被打断
os使用了最低等级的异常，用来调度进程。所以在中断里处理i2c也是可行的，
但会降低系统的响应速度
*/
#include "user_common.h"
#include "stm32f10x.h"	
	
#define HARDWAREIIC 1
#define SOFTWAREIIC 1
#define TRUE 1
#define FALSE 0
#if(SOFTWAREIIC ==1)
//************************************
/*模拟IIC端口输出输入定义*/
#define SCL_PORT_H(PORT,PIN)		GPIO##PORT->BSRR = GPIO_Pin_##PIN
#define SCL_PORT_L(PORT,PIN)		GPIO##PORT->BRR = GPIO_Pin_##PIN
#define SDA_PORT_H(PORT,PIN)		GPIO##PORT->BSRR = GPIO_Pin_##PIN
#define SDA_PORT_L(PORT,PIN)		GPIO##PORT->BRR = GPIO_Pin_##PIN 
#define SCL_PORT_read(PORT,PIN)		GPIO##PORT->IDR  & GPIO_Pin_##PIN
#define SDA_PORT_read(PORT,PIN)		GPIO##PORT->IDR  & GPIO_Pin_##PIN

#define SCL_H         SCL_PORT_H(B,6)
#define SCL_L         SCL_PORT_L(B,6)
 
#define SDA_H         SDA_PORT_H(B,7)
#define SDA_L         SDA_PORT_L(B,7)

#define SCL_read      SCL_PORT_read(B,6)
#define SDA_read      SDA_PORT_read(B,7)
typedef int bool;
typedef struct{
	void(*init)(GPIO_TypeDef *SCL_GPIOx,uint16_t SCL_PIN,GPIO_TypeDef *SDA_GPIOx,uint16_t SDA_PIN);
	bool (*start)();
	void (*stop)();
	bool(*sendbyte)(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
	unsigned char (*recebyte)(unsigned char SlaveAddress,unsigned char REG_Address);
}Soft_I2c_t;
extern const Soft_I2c_t soft_iic;
#endif

#if(HARDWAREIIC ==1)

#endif
#ifdef __cplusplus
}
#endif
#endif