#include "arch/arm-cortex-m3/arch.h"
#include "core/task.h"
#include "stm32f10x.h"
//设置systick
//设置中断屏蔽和优先级
//设置msp和psp
//设置pendsv，产生调度信号
#include "misc.h"
void system_tick_config(){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_Config(9000);//1ms
}
__asm void arch_start_task(){
	
	PRESERVE8
	/*取向量表起始地址的内容*/
	ldr r0,=0xE000ED08
	ldr r0,[r0]
	ldr r0,[r0]
	/*设置MSP*/
	msr msp,r0
	/*使能中断*/
	cpsie i
	cpsie f
	dsb
	isb
	/*产生0号系统调用*/
	svc 0
	nop
	nop
}
#define portNVIC_SYSPRI2_REG				( * ( ( volatile unsigned int * ) 0xe000ed20 ) )
void arch_oneos_start(){
	system_tick_config();
	/* 配置 PendSV 和 SysTick 的中断优先级为最低 */
	portNVIC_SYSPRI2_REG |= 255;
	portNVIC_SYSPRI2_REG |= 255;
	//开始第一个任务，不再返回
	arch_start_task();
}

extern void task_err_fun();
//初始化task的堆栈,并且返回栈指针
void* arch_initTask_stack(void *stack_p,void(*task_fun)(void)){
	unsigned int * p = (unsigned int *)stack_p;
	p--;
	*p=0x01000000;
	p--;
	*p=(unsigned int)((unsigned int)task_fun&(unsigned int)(0xfffffffe));
	p--;
	*p=(unsigned int)task_err_fun;
	p-=5;
	p-=8;
	return (void *)p;
}
//负责挑选cur_task 调用上层的调度算法
extern err_t schedule();
void arch_task_schedule(){
	schedule();
}

//当前任务放弃cpu，pending调度中断(即产生pendsv)
#define portNVIC_INT_CTRL_REG		( * ( ( volatile unsigned int * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )
#define portSY_FULL_READ_WRITE		( 15 )
void arch_task_yield(){
		/* Set a PendSV to request a context switch. */								\
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;								\
																				\
	/* Barriers are normally not required but do ensure the code is completely	\
	within the specified behaviour for the architecture. */						\
	__dsb( portSY_FULL_READ_WRITE );											\
	__isb( portSY_FULL_READ_WRITE );	
}
#define MAX_SYSCALL_INTERUPT_PRIORITY 0
//负责上下文的切换
__asm void arch_context_switch(){
	extern cur_task;
	extern arch_task_schedule
	PRESERVE8
	/*获取当前上文的栈指针*/
	mrs r0,psp
	isb
	/*获取当前上文的栈指针*/
	ldr r3,=cur_task
	ldr r2,[r3]
	/*保存上文的r4-r11*/
	stmdb r0!,{r4-r11}
	/*上文的栈指针值保存*/
	str r0,[r2]
	/*切换到下文,保存r3和r14到msp*/
	stmdb sp!,{r3,r14}
	/*失能部分中断*/
	mov r0,#MAX_SYSCALL_INTERUPT_PRIORITY
	msr basepri,r0
	dsb
	isb
	/*切换cur_task*/
	bl arch_task_schedule
	mov r0,#0
	msr basepri,r0
	/*从msp里弹出r3和r14,r3保存的是cur_task的地址
	在刚刚这个地址处的值已经被更新了，所以取新的cur_task*/
	ldmia sp!,{r3,r14}
	ldr r1,[r3]
	ldr r0,[r1]
	/*弹出新的cur_task的栈环境到r4-r11*/
	ldmia r0!,{r4-r11}
	/*更新栈指针*/
	msr psp,r0
	isb
	/*自动跳转到新的cur_task*/
	bx r14
	nop
}
//滴答定时器
void SysTick_Handler(){
	//每个时基调度一次
	arch_task_yield();
}

//挂上系统调用
//PendSV是为系统级服务提供的中断驱动。\
在一个操作系统环境中，当没有其他异常正在执行时，\
可以使用PendSV来进行上下文的切换。
void PendSV_Handler(){
	/*和体系结构相关的任务切换
	这里完成了上文的保存，下个任务的挑选，和下文的装入*/
	arch_context_switch();
}	
//处理svc指令的系统调用
/*在这里装入当前task的环境，并且切换到线程模式和psp*/
__asm void SVC_Handler(){
	
	extern cur_task;
	
	PRESERVE8
	
	ldr r3,=cur_task
	ldr r1,[r3]
	/*取task栈顶指针的值到r0*/
	ldr r0,[r1]
	/*加载栈内容到r4~r11*/
	ldmia r0!,{r4-r11}
	msr psp,r0
	isb
	mov r0,#0
	/*设置中断屏蔽字*/
	msr basepri,r0
	/*设置lr寄存器，使内核进入线程态，使用psp*/
	orr r14,#0x0d
	/*返回，进入task*/
	bx r14
}