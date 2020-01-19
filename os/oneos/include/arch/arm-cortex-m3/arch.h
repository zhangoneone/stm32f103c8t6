#ifndef ARCH_H
#define ARCH_H
#ifdef __cplusplus
extern "C"{
#endif
#define DEBUG

#ifndef NULL
#define NULL		0x00
#endif

#define TRUE		1
#define FALSE		0
#ifndef ONEMALLOC(size)
#define ONEMALLOC(size)	one_malloc(size)
#endif
#ifndef ONEFREE(addr,size)
#define ONEFREE(addr,size)				one_free_size(addr,size) //>>>>>
#endif
extern void xprintf (const char*	fmt,...);
#if defined(DEBUG)
#define DEBUG_LOG(char,int)		xprintf("assert failed:%s,%d",char,int)	
#define assert(expr)			if((expr)==0) DEBUG_LOG(__FILE__,__LINE__)
#endif
typedef enum{
	err_ok=0,
	err_failed=1,
}err_t;

typedef unsigned char  uchar;   
typedef unsigned int   uint; 
typedef unsigned long  ulong;
typedef unsigned char bool;
typedef unsigned int size_t;
#define offset(TYPE,MEMBER)	((size_t)&(((TYPE *)0)->MEMBER))
//给定结构类型，成员名称，成员地址，求结构的起始地址
#define contain_of(TYPE,MEMBER,MEMBER_PTR) ((TYPE *)(MEMBER_PTR-offset(TYPE,MEMBER)))
typedef uchar sem_t;//计数型信号量
#define sem_init(sem_name,value)		sem_name = value
#define sem_wait(sem_name)				while(!sem_name);	\
										sem_name-=1	
#define sem_post(sem_name)				sem_name+=1

typedef bool mutex_t; //互斥量
#define mutex_init(mutex_name,value)	mutex_name = value
#define mutex_lock(mutex_name)			while(!mutex_name);	\
										mutex_name=0	
#define mutex_unlock(mutex_name)		mutex_name=1
//临界区 关闭全部中断

#define critical_area_enter()				
#define critical_area_exit()				

#define ban_schedule()						
#define allow_schedule()					
void* arch_initTask_stack(void *stack_p,void(*task_fun)(void));
void arch_oneos_start();
void arch_task_yield();
void system_tick_config();
#ifdef __cplusplus
}
#endif
#endif