#include "user_crc.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_rcc.h"
//crc主要是两个数据寄存器和一个控制寄存器。其中一个数据寄存器分为两组，分别是\
只读和只写\
crc生成多项式是固定的
//crc的结果可以迭代，只要不清空，不停的迭代输入，输入结束再返回crc结果。\
算出来的就是整个数据的结果\
stm32的CRC校验和主流的计算结果不一致，是大小端的问题\
以及计算结果没有异或
void user_crc_init(){
	//打开时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
	//清空CRC寄存器
	CRC_ResetDR();
}
uint32 user_cal_crc_32(uint32 data){
	uint32 ret;
	ret = CRC_CalcCRC(data);
	//清空CRC寄存器
	CRC_ResetDR();
	//返回结果
	return ret;
}
//数据块指针，数据块长度
//返回CRC校验码
uint32 user_cal_crc(uint32 *ptr,uint32 len){
	uint32 ret;
	ret = CRC_CalcBlockCRC(ptr,len);
	//清空CRC寄存器
	CRC_ResetDR();
	//返回结果
	return ret;
}