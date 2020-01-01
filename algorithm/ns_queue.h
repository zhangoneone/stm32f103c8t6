#ifndef NS_QUEUE_H
#define NS_QUEUE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_algorithm.h"
	
#define STATIC_QUEUE 0
#define STATIC_CYCLE_QUEUE 0
#define MAX_QUEUE_LEN	100
typedef element generic_static_queue_load[MAX_QUEUE_LEN];//静态队列
typedef struct {
	generic_static_queue_load load;
	int queue_head;//这里head指向存放队列头结点的前一个结点。
	int queue_tail;//这里tail指向存放队列尾结点。
}generic_static_queue;

typedef struct {
	generic_static_queue_load load;
	int queue_head;//这里head指向存放队列头结点的前一个结点。
	int queue_tail;//这里tail指向存放队列尾结点。
}generic_static_cycle_queue;//数组定义的循环队列

typedef struct {
	generic_static_queue_load load;
	int queue_left;//这里head指向存放队列头结点的前一个结点。
	int queue_right;//这里tail指向存放队列尾结点。
}generic_static_double_queue;//数组定义的双向队列

#ifdef __cplusplus
}
#endif
#endif

