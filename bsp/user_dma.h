#ifndef USER_DMA_H
#define USER_DMA_H
#ifdef __cplusplus
extern "C" {
#endif
#include "user_common.h"
#include "stm32f10x.h"

typedef struct{
	void (*init)(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
	void (*start)(DMA_Channel_TypeDef*DMA_CHx,uint transmit_counter);
	void(*stop)(DMA_Channel_TypeDef*DMA_CHx);
}Dma_t;
extern const Dma_t dma1;
#if ZET6 == 1
extern const Dma_t dma2;
#endif
extern volatile uint failed_counter[7+5];//传输出错时，这里会保存尚未发完的数据量大小
#ifdef __cplusplus
}
#endif
#endif