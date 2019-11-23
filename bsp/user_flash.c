#include "user_flash.h"
#include "stm32f10x_flash.h"
/*因为我们移植的fatfs需要diskio的支持，所以我们需要在flash操作函数里面，实现这些
diskio的函数，这样才能进一步移植fatfs*/
//实现如下几个函数外加rtc获取时间函数，即可使用fatfs了
DSTATUS flash_initialize(BYTE pdrv){
	
	return RES_OK;
}

DSTATUS flash_status(BYTE pdrv){
	
	return RES_OK;
}

DRESULT flash_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count){
	
	return RES_OK;
}

DRESULT flash_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
	
	return RES_OK;
}

DRESULT flash_ioctl(BYTE pdrv, BYTE cmd, void* buff){
	
	return RES_OK;
}

const struct flash_driver flash_driver={
	flash_initialize,
	flash_status,
	flash_read,
	flash_write,
	flash_ioctl,
};