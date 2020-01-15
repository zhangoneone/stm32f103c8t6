#ifndef MEM_H
#define MEM_H
#ifdef __cplusplus
extern "C"{
#endif
#include "arch/arm-cortex-m3/arch.h"
#include "oneosConfig.h"
#define MEM_SIZE		HEAP_SIZE
	
#if (BIT_MAP==1)
#define MIN_ALLOC_BYTE	4	
#define BITMAP_SIZE (MEM_SIZE/(8*MIN_ALLOC_BYTE))
void one_free_size(void *addr,uint size);
#endif
void *one_malloc(uint size);
void one_memset(void *addr,uint value,uint size);
void one_memcpy(void *dest,void *src,uint size);
#ifdef __cplusplus
}
#endif
#endif