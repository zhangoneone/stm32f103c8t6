#ifndef USER_FLASH_H
#define USER_FLASH_H

#include "user_common.h"
/*@brief
stm32内部的rom有2处：
flash 64K~256K不等
信息块(系统存储器2K Byte 用户选择字节 16 Byte

内部ram有1处，即sram。

联系到boot的3种方式：
系统存储器boot
flash存储器boot
sram存储器boot
一般都是把程序下载到flash里面。
*/

/*@brief
我们想移植fs
128K的flash一共有128个块，每个块有1K。这个类似与sector(扇区)
除去下载到flash中的代码和只读数据，剩下的flash大小，就是我们可以操作的范围了
*/

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;
typedef BYTE DSTATUS;
struct flash_driver{
	DSTATUS (*initialize)(BYTE pdrv);
	DSTATUS (*status)(BYTE pdrv);
	DRESULT (*read)(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
	DRESULT (*write)(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
	DRESULT (*ioctl)(BYTE pdrv, BYTE cmd, void* buff);
};

extern const struct flash_driver flash_driver;
#endif