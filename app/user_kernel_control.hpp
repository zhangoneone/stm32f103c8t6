#ifndef USER_KERNEL_CONTROL_H
#define USER_KERNEL_CONTROL_H
#ifdef __cplusplus
extern "C" {
#endif
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
extern xTaskHandle Kernel_TASK_PCB;//KernelÏß³Ì¾ä±ú
extern void kernel_test_start();
class Kernel{
	public:
		Kernel();
		~Kernel();
	static void top_command();
	friend void kernel_test_start();
	protected:

	private:
		
};

#ifdef __cplusplus
}
#endif
#endif