#ifndef USER_I2C_H
#define USER_I2C_H
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

#endif