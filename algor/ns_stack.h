#ifndef NS_STACK_H
#define NS_STACK_H
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_algorithm.h"
#define STATIC_STACK 0
#define MAX_STACK_LEN	100
typedef element generic_static_stack_load[MAX_STACK_LEN];//¾²Ì¬Õ»
typedef struct {
	generic_static_stack_load load;
	int stack_top;
}generic_static_stack;

#ifdef __cplusplus
}
#endif
#endif
