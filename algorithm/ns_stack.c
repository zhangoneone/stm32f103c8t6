#include "ns_stack.h"
/********静态栈部分**************/
#if(STATIC_STACK == 1)
/***********静态栈私有变量********************/
//栈空时top指针为0，满时top指针是max+1
static status generic_static_stack_empty(generic_static_stack *s) {
	if (s->stack_top == 0)return true;
	else return false;
}
static status generic_static_stack_full(generic_static_stack *s) {
	if (s->stack_top == MAX_STACK_LEN + 1)return true;
	else return false;
}
/***********静态栈全局变量********************/
status generic_static_stack_push(generic_static_stack *s,element x) {
	//这种写法，需要先放数据，再top++
	element *ptr = (element *)s->load;
	if (generic_static_stack_full(s))return -1;
	s->load[s->stack_top++] = x;
	return 0;
}

status generic_static_stack_pop(generic_static_stack *s, element *x) {
	//这种写法，需要先top--，再出数据
	element *ptr = (element *)s->load;
	if (generic_static_stack_empty(s))return -1;
	*x = s->load[--s->stack_top];
	return 0;
}

#endif