/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			0
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )	
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 5 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) )
#define configMAX_TASK_NAME_LEN		( 16 )
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

//以下是自己定义的配置，可能和上面官方的配置有重复的地方

//针对不同编译器选择不同stdin.h
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif
extern void xprintf_s(const char*	fmt,...);
//断言
#define vAssertCalled(char,int) xprintf_s("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)

//断言
#ifdef  DEBUG
/* Keep the linker happy. */
void assert_failed( unsigned char* pcFile, unsigned long ulLine )
{
	for( ;; )
	{
	}
}
#endif

//1 使用抢占式调度 0 使用协作式调度 （时间片）
#define configUSE_PREEMPTION 1

//使能时间片调度
#define configUSE_TIME_SLICING 1

//使用平台相关的上下文切换方式，提高效率
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

//1:使能tickless模式 0:时钟( tick)一直保持运行
#define configUSE_TICKLESS_IDLE 0

//cpu主频传递给freertos内核，这里是72MHZ
#define configCPU_CLOCK_HZ (SystemCoreClock)

//系统节拍中断频率。每次中断都会进行任务调度。这里是1000HZ，即1ms调度1次，时间片是1ms
#define configTICK_RATE_HZ (( TickType_t )1000)

//可使用的最大优先级
#define configMAX_PRIORITIES (32)

//空闲任务堆栈大小
#define configMINIMAL_STACK_SIZE ((unsigned short)128)
	
//任务名称字符串长度最大值
#define configMAX_TASK_NAME_LE (16)

//系统节拍计数器变量数据类型 1 16位无符号整型 0 32位无符号整型
#define configUSE_16_BIT_TICKS 0

//空闲任务放弃cpu，给其他相同优先级任务
#define configIDLE_SHOULD_YIELD 1

//启用队列
#define configUSE_QUEUE_SETS 1

//开启任务通知功能， 默认开启
#define configUSE_TASK_NOTIFICATIONS 1

//启用互斥信号量
#define configUSE_MUTEXES 1

//启用递归互斥信号量
#define configUSE_RECURSIVE_MUTEXES 1

//启用计数信号量
#define configUSE_COUNTING_SEMAPHORES 1

//信号量和消息队列最大可以注册的数量
#define configQUEUE_REGISTRY_SIZE 10
#define configUSE_APPLICATION_TASK_TAG 0

/*内存相关配置*/
//支持动态内存
#define configSUPPORT_DYNAMIC_ALLOCATION 1

//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION 0

//系统所有总的堆大小 注意编译之后不能让变量占用内存超过芯片sram大小
#define configTOTAL_HEAP_SIZE ((size_t)(10*1024))

//钩子函数设置 回调函数 
//空闲钩子函数 void vApplicationIdleHook(void ) 每个空闲任务执行期都会调用
#define configUSE_IDLE_HOOK 0

//时间片钩子函数 void vApplicationTickHook(void )
#define configUSE_TICK_HOOK 0

//内存申请失败钩子函数
#define configUSE_MALLOC_FAILED_HOOK 0

//堆栈溢出检测钩子 使用的话，设置成 1 2，因为有2种检测方式
#define configCHECK_FOR_STACK_OVERFLOW 0

//任务状态和运行状态收集
//运行时间统计
#define configGENERATE_RUN_TIME_STATS 0

//可视化追踪调试
#define configUSE_TRACE_FACILITY 0
/* 与宏 configUSE_TRACE_FACILITY 同时为1时会编译如下3个函数
prvWriteNameToBuffer()
vTaskList(),
vTaskGetRunTimeStats()
*/

#define configUSE_STATS_FORMATTING_FUNCTIONS 1

//启用协程
#define configUSE_CO_ROUTINES 0

//协程的有效优先级数量
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

//启用软件定时器
#define configUSE_TIMERS 1
//软件定时器优先级
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES-1)
//软件定时器队列长度
#define configTIMER_QUEUE_LENGTH 10
//软件定时器任务堆栈大小
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE*2)

//可选的函数参数配置
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskCleanUpResources 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1

//中断有关配置

#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 4
#endif
//中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configKERNEL_INTERRUPT_PRIORITY	\
( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY	\
( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

//中断服务函数配置
//配置宏，把以下三个中断交给freertos管理
#define xPortPendSVHandler PendSV_Handler 
#define xPortSysTickHandler SysTick_Handler 
#define vPortSVCHandler SVC_Handler

/*Percepio Tracealyzer  configUSE_TRACE_FACILITY 为1时需要的*/
#if ( configUSE_TRACE_FACILITY == 1 )
#include "trcRecorder.h"
#define INCLUDE_xTaskGetCurrentTaskHandle 0
#endif
#endif /* FREERTOS_CONFIG_H */

