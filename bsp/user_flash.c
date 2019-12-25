#include "user_flash.h"
#include "stm32f10x_flash.h"
/*因为我们移植的fatfs需要diskio的支持，所以我们需要在flash操作函数里面，实现这些
diskio的函数，这样才能进一步移植fatfs*/
//实现如下几个函数外加rtc获取时间函数，即可使用fatfs了
/********初始化flash设备************/
//设置延迟周期，72MHZ主频的延迟时钟周期为2。表示cpu发出\
flash操作指令后，2周期后再访问flash的状态
DSTATUS flash_initialize(BYTE pdrv){
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);//要关闭半周期访问
	return RES_OK;
}

DSTATUS flash_status(BYTE pdrv){
	//初始化不成功
	if(flash_initialize(0))
		return STA_NOINIT;
	//初始化成功，检查写保护
	if((FLASH_GetWriteProtectionOptionByte()&FLASH_WRProt_Pages62to127) != FLASH_WRProt_Pages62to127)
		return STA_PROTECT;
	else 
		return RES_OK;
}

DRESULT flash_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count){
	UINT addr = FLASH_START_ADDR;
	int i = 0;
	//入参检查
	if(!buff || sector+count > FLASH_SECTOR_NUMBER)
		return RES_PARERR;
	//设备检查
	if(flash_initialize(0))
		return RES_NOTRDY;
	//通过转换地址直接读
	sector+=USER_START_SECTOR;//设置扇区偏移
	addr += FLASH_SECTOR_SIZE*sector;
	count *= FLASH_SECTOR_SIZE;
	for(;i<count;i++){
		buff[i] = *(__IO BYTE *)addr;
		addr++;
	}
	return RES_OK;
}

#if USE_FAT_FS ==0 
DRESULT flash_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
	//stm32支持半字和全字写入
	UINT data = 0x0;
	UINT addr = FLASH_START_ADDR;
	UINT i =0;
	//入参检查
	if(!buff || sector+count > FLASH_SECTOR_NUMBER)
		return RES_PARERR;
	//设备检查
	if(flash_initialize(0))
		return RES_NOTRDY;
	//写保护检查
	if((FLASH_GetWriteProtectionOptionByte()&FLASH_WRProt_Pages62to127) != FLASH_WRProt_Pages62to127)
		return RES_WRPRT;
	//地址转换
	sector+=USER_START_SECTOR;//设置扇区偏移
	addr += FLASH_SECTOR_SIZE*sector;
	//解锁
	FLASH_Unlock();
	//批量擦除指定扇区
	for(;i<count;i++){
		if(FLASH_COMPLETE != FLASH_ErasePage(addr+i*FLASH_SECTOR_SIZE)){
			//assert_param(0);
			return RES_ERROR;
		}
	}
	//设置待写入字节数量
	count *= FLASH_SECTOR_SIZE;
	//批量写入数据
	for(i=0;i<count;i+=4,addr += 4){
		data = buff[i] | (buff[i+1]<<8) | (buff[i+2]<<16) | (buff[i+3]<<24); //小端存储。低地址保存低字节
		if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data)){
			//assert_param(0);
			return RES_ERROR;
		}
	}
	//上锁
	FLASH_Lock();
	return RES_OK;
}

#elif USE_FAT_FS ==1
DRESULT flash_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
	//stm32支持半字和全字写入
	BYTE swap_buff[FLASH_SECTOR_SIZE*2];//交换缓冲
	UINT data = 0x0;
	UINT addr = FLASH_START_ADDR;
	UINT i =0,j=0;
	UINT front = 0,back=0;//0代表对齐，1代表不对齐
	//入参检查
	if(!buff || sector+count > FLASH_SECTOR_NUMBER)
		return RES_PARERR;
	//设备检查
	if(flash_initialize(0))
		return RES_NOTRDY;
	//写保护检查
	if((FLASH_GetWriteProtectionOptionByte()&FLASH_WRProt_Pages62to127) != FLASH_WRProt_Pages62to127)
		return RES_WRPRT;
	//地址转换
	sector+=USER_START_SECTOR;//设置扇区偏移
	addr += FLASH_SECTOR_SIZE*sector;
	front = (sector&0x1)?1:0;
	back = (sector+count-1)&0x1?0:1;
	//解锁
	FLASH_Unlock();
	//处理前后对齐
	if(front){
		flash_read(pdrv,swap_buff,sector-1,1);
	//	if(FLASH_COMPLETE != FLASH_ErasePage(addr-FLASH_SECTOR_SIZE))return RES_ERROR;
	}
	if(back){
		flash_read(pdrv,swap_buff+FLASH_SECTOR_SIZE,sector+count,1);
	//	if(FLASH_COMPLETE != FLASH_ErasePage(addr+FLASH_SECTOR_SIZE*(sector+count-1)))return RES_ERROR;
	}
	//批量擦除扇区
	for(;i<=count;i+=2){//==是前后均不对齐的情况。因为前不对齐时，左移了擦除地址。导致后不对齐变成对齐
		if(FLASH_COMPLETE != FLASH_ErasePage(addr+i*FLASH_SECTOR_SIZE-
																																(front?FLASH_SECTOR_SIZE:0)))
			return RES_ERROR;
	}
	//设置待写入字节数量
	count *= FLASH_SECTOR_SIZE;
	//重定位写入地址
	addr-=front*FLASH_SECTOR_SIZE;
	if(front){//补回前对齐
		for(i=0;i<FLASH_SECTOR_SIZE;i+=4,addr+=4){
			data = swap_buff[i] | (swap_buff[i+1]<<8) | (swap_buff[i+2]<<16) | (swap_buff[i+3]<<24); //小端存储。低地址保存低字节
			if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data))return RES_ERROR;
		}
	}
	//批量写入数据
	for(i=0;i<count;i+=4,addr += 4){
		data = buff[i] | (buff[i+1]<<8) | (buff[i+2]<<16) | (buff[i+3]<<24); //小端存储。低地址保存低字节
		if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data))return RES_ERROR;
	}
	if(back){//补回后对齐
		for(i=FLASH_SECTOR_SIZE;i<2*FLASH_SECTOR_SIZE;i+=4,addr+=4){
			data = swap_buff[i] | (swap_buff[i+1]<<8) | (swap_buff[i+2]<<16) | (swap_buff[i+3]<<24); //小端存储。低地址保存低字节
			if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data))return RES_ERROR;
		}
	}
	//上锁
	FLASH_Lock();
	return RES_OK;
}




#endif
DRESULT flash_ioctl(BYTE pdrv, BYTE cmd, void* buff){
	//设备检查
	if(flash_initialize(0))
		return RES_NOTRDY;
	if(cmd == CTRL_SYNC){
	  return RES_OK;
	}
	else if(cmd == GET_SECTOR_COUNT){
		if(!buff)return RES_ERROR;
		*(UINT*)buff = FLASH_SECTOR_NUMBER;
	}
	else if(cmd == GET_SECTOR_SIZE){
		if(!buff)return RES_ERROR;
		*(UINT*)buff = FLASH_SECTOR_SIZE;
	}
	else if(cmd == GET_BLOCK_SIZE){
		if(!buff)return RES_ERROR;
		*(UINT*)buff = FLASH_BLOCK_SIZE;
	}
	else if(cmd == CTRL_TRIM){//通知某个范围的逻辑区块不再需要,可以擦除
		 return RES_OK;
	}
	else
		return RES_PARERR;
	return RES_OK;
}

/*@brief:返回flash大小，单位：KB
*/
unsigned short flash_size(void){
	return ((*((unsigned short *)0x1FFFF7E0))&0xFFFF);
}
const struct flash_driver flash_driver={
	flash_initialize,
	flash_status,
	flash_read,
	flash_write,
	flash_ioctl,
};