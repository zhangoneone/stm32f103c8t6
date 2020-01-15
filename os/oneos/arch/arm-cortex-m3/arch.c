#include "arch/arm-cortex-m3/arch.h"
#include "core/task.h"

//����systick
//�����ж����κ����ȼ�
//����msp��psp
//����pendsv�����������ź�
__asm void arch_oneos_start(){
	/* ���� PendSV �� SysTick ���ж����ȼ�Ϊ��� */
//	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
//	portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
	PRESERVE8
	/*ȡ��������ʼ��ַ������*/
	ldr r0,=0xE000ED08
	ldr r0,[r0]
	ldr r0,[r0]
	/*����MSP*/
	msr msp,r0
	/*ʹ���ж�*/
	cpsie i
	cpsie f
	dsb
	isb
	/*����0��ϵͳ����*/
	svc 0
	nop
	nop
}
//��ʼ��task�Ķ�ջ
void arch_initTask_stack(void *stack_p,void(*task_fun)(void)){
	unsigned int * p = (unsigned int *)stack_p;
	p--;
	//*p=xpsr;
	p--;
	*p=(unsigned int)task_fun;
	p--;
	p--;
	p-=5;
	p-=8;
	stack_p=(void *)p;
	return;
}
//������ѡcur_task �����ϲ�ĵ����㷨
extern err_t schedule();
void arch_task_schedule(){
	schedule();
}

//��ǰ�������cpu��pending�����ж�(������pendsv)
void arch_task_yield(){
	
}
#define MAX_SYSCALL_INTERUPT_PRIORITY 0
//���������ĵ��л�
__asm void arch_context_switch(){
	extern cur_task;
	extern arch_task_schedule
	PRESERVE8
	/*��ȡ��ǰ���ĵ�ջָ��*/
	mrs r0,psp
	isb
	/*��ȡ��ǰ���ĵ�ջָ��*/
	ldr r3,=cur_task
	ldr r2,[r3]
	/*�������ĵ�r4-r11*/
	stmdb r0!,{r4-r11}
	/*���ĵ�ջָ��ֵ����*/
	str r0,[r2]
	/*�л�������,����r3��r14��msp*/
	stmdb sp!,{r3,r14}
	/*ʧ�ܲ����ж�*/
	mov r0,#MAX_SYSCALL_INTERUPT_PRIORITY
	msr basepri,r0
	dsb
	isb
	/*�л�cur_task*/
	bl arch_task_schedule
	mov r0,#0
	msr basepri,r0
	/*��msp�ﵯ��r3��r14,r3�������cur_task�ĵ�ַ
	�ڸո������ַ����ֵ�Ѿ��������ˣ�����ȡ�µ�cur_task*/
	ldmia sp!,{r3,r14}
	ldr r1,[r3]
	ldr r0,[r1]
	/*�����µ�cur_task��ջ������r4-r11*/
	ldmia r0!,{r4-r11}
	/*����ջָ��*/
	msr psp,r0
	isb
	/*�Զ���ת���µ�cur_task*/
	bx r14
	nop
}
//�δ�ʱ��
void SysTick_Handler(){
	//ÿ��ʱ������һ��
	arch_task_yield();
}

//����ϵͳ����
//PendSV��Ϊϵͳ�������ṩ���ж�������\
��һ������ϵͳ�����У���û�������쳣����ִ��ʱ��\
����ʹ��PendSV�����������ĵ��л���
void PendSV_Handler(){
	/*����ϵ�ṹ��ص������л�
	������������ĵı��棬�¸��������ѡ�������ĵ�װ��*/
	arch_context_switch();
}	
//����svcָ���ϵͳ����
/*������װ�뵱ǰtask�Ļ����������л����߳�ģʽ��psp*/
__asm void SVC_Handler(){
	
	extern cur_task;
	
	PRESERVE8
	
	ldr r3,=cur_task
	ldr r1,[r3]
	/*ȡtaskջ��ָ���ֵ��r0*/
	ldr r0,[r1]
	/*����ջ���ݵ�r4~r11*/
	ldmia r0!,{r4-r11}
	msr psp,r0
	isb
	mov r0,#0
	/*�����ж�������*/
	msr basepri,r0
	/*����lr�Ĵ�����ʹ�ں˽����߳�̬��ʹ��psp*/
	orr r14,#0x0d
	/*���أ�����task*/
	bx r14
}