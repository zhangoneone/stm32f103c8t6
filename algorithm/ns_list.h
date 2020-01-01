#ifndef NS_LIST_H
#define NS_LIST_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "ns_algorithm.h"
#define  STATIC_LIST 0
#define  DYNAMIC_LIST 0
#define MAX_LIST_LEN		100
typedef element generic_static_list[MAX_LIST_LEN];//静态表

typedef struct generic_dynamic_single_list {
	element value;
	struct generic_dynamic_single_list *next;
}generic_dynamic_single_list;//单链表

typedef struct generic_dynamic_double_list {
	element value;
	struct generic_dynamic_double_list *pre,*next;
}generic_dynamic_double_list;//双向链表

typedef struct generic_dynamic_double_cycle_list {
	element value;
	struct generic_dynamic_double_list *pre, *next;
}generic_dynamic_double_cycle_list;//双向循环链表

#ifdef __cplusplus
}
#endif

#endif
