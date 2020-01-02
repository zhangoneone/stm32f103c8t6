#include "user_dma.h"
 
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
static void user_Dma_init(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar){
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输时钟
#if ZET6==1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA传输时钟
#endif
  DMA_DeInit(DMA_CHx);   //将DMA的通道x寄存器重设为缺省值

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 0;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器  	

	//DMA的NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
#if ZET6 == 1
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
#endif
} 
//开启一次DMA传输
static void user_Dma_Enable(DMA_Channel_TypeDef*DMA_CHx,uint transmit_counter){ 
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道 
	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);	//传输完成中断
	DMA_ITConfig(DMA_CHx, DMA_IT_TE, ENABLE);	//传输出错中断
 	DMA_SetCurrDataCounter(DMA_CHx,transmit_counter);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	  
//关闭DMA传输
static void user_Dma_Disable(DMA_Channel_TypeDef*DMA_CHx){
	DMA_Cmd(DMA_CHx, DISABLE );
}

volatile uint failed_counter[7+5]={0,0,0,0,0,0,0,0,0,0,0,0};
void DMA1_Channel1_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC1);//清除通道传输完成标志
		failed_counter[0] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE1) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE1);//清除通道传输出错标志
		failed_counter[0] = DMA_GetCurrDataCounter(DMA1_Channel1);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL1);//清中断
}

void DMA1_Channel2_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC2);//清除通道传输完成标志
		failed_counter[1] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE2) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE2);//清除通道传输出错标志
		failed_counter[1] = DMA_GetCurrDataCounter(DMA1_Channel2);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL2);//清中断	
}
void DMA1_Channel3_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道传输完成标志
		failed_counter[2] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE3) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE3);//清除通道传输出错标志
		failed_counter[2] = DMA_GetCurrDataCounter(DMA1_Channel3);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL3);//清中断	
}
void DMA1_Channel4_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道传输完成标志
		failed_counter[3] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE4) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE4);//清除通道传输出错标志
		failed_counter[3] = DMA_GetCurrDataCounter(DMA1_Channel4);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL4);//清中断	
}
void DMA1_Channel5_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC5);//清除通道传输完成标志
		failed_counter[4] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE5) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE5);//清除通道传输出错标志
		failed_counter[4] = DMA_GetCurrDataCounter(DMA1_Channel5);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL5);//清中断	
}
void DMA1_Channel6_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC6) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC1);//清除通道传输完成标志
		failed_counter[5] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE6) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE6);//清除通道传输出错标志
		failed_counter[5] = DMA_GetCurrDataCounter(DMA1_Channel6);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL6);//清中断	
}
void DMA1_Channel7_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7) == SET){//传输完成
		DMA_ClearFlag(DMA1_FLAG_TC7);//清除通道传输完成标志
		failed_counter[6] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE7) == SET){//传输出错
		DMA_ClearFlag(DMA1_FLAG_TE7);//清除通道传输出错标志
		failed_counter[6] = DMA_GetCurrDataCounter(DMA1_Channel7);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA1_FLAG_GL7);//清中断	
}


void DMA2_Channel1_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA2_FLAG_TC1) == SET){//传输完成
		DMA_ClearFlag(DMA2_FLAG_TC1);//清除通道传输完成标志
		failed_counter[7] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE1) == SET){//传输出错
		DMA_ClearFlag(DMA2_FLAG_TE1);//清除通道传输出错标志
		failed_counter[7] = DMA_GetCurrDataCounter(DMA2_Channel1);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA2_FLAG_GL1);//清中断	
}
void DMA2_Channel2_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA2_FLAG_TC2) == SET){//传输完成
		DMA_ClearFlag(DMA2_FLAG_TC2);//清除通道传输完成标志
		failed_counter[8] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE2) == SET){//传输出错
		DMA_ClearFlag(DMA2_FLAG_TE2);//清除通道传输出错标志
		failed_counter[8] = DMA_GetCurrDataCounter(DMA2_Channel2);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA2_FLAG_GL2);//清中断	
}
void DMA2_Channel3_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA2_FLAG_TC3) == SET){//传输完成
		DMA_ClearFlag(DMA2_FLAG_TC3);//清除通道传输完成标志
		failed_counter[9] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE3) == SET){//传输出错
		DMA_ClearFlag(DMA2_FLAG_TE3);//清除通道传输出错标志
		failed_counter[9] = DMA_GetCurrDataCounter(DMA2_Channel3);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA2_FLAG_GL3);//清中断	
}

void DMA2_Channel4_5_IRQHandler(){
	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA2_FLAG_TC4) == SET){//传输完成
		DMA_ClearFlag(DMA2_FLAG_TC4);//清除通道传输完成标志
		failed_counter[10] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE4) == SET){//传输出错
		DMA_ClearFlag(DMA2_FLAG_TE4);//清除通道传输出错标志
		failed_counter[10] = DMA_GetCurrDataCounter(DMA1_Channel4);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA2_FLAG_GL4);//清中断	

	//读取标志位，查看dma传输情况
	if(DMA_GetFlagStatus(DMA2_FLAG_TC5) == SET){//传输完成
		DMA_ClearFlag(DMA2_FLAG_TC5);//清除通道传输完成标志
		failed_counter[11] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE5) == SET){//传输出错
		DMA_ClearFlag(DMA2_FLAG_TE5);//清除通道传输出错标志
		failed_counter[11] = DMA_GetCurrDataCounter(DMA1_Channel5);//得到当前还剩余多少个数据
	}
	DMA_ClearFlag(DMA2_FLAG_GL5);//清中断
}


const Dma_t dma1 = {
	user_Dma_init,
	user_Dma_Enable,
	user_Dma_Disable,
};
const Dma_t dma2 = {
	user_Dma_init,
	user_Dma_Enable,
	user_Dma_Disable,
};

//demo:
/*
//DMA1通道4,外设为串口1,存储器为SendBuff,长度SEND_BUF_SIZE.
user_Dma_init(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送  库函数    
user_Dma_Enable(DMA1_Channel4);//开始一次DMA传输！	 
if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)	//判断通道4传输完成
  DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
DMA_GetCurrDataCounter(DMA1_Channel4);//得到当前还剩余多少个数据

*/



