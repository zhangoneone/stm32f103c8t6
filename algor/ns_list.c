#include "ns_list.h"
/********静态表部分**************/
#if (STATIC_LIST == 1)
/***********静态表私有变量********************/


/***********静态表全局变量********************/
//静态表插入 在有list_len个元素的表L的index位置，插入x。index从0开始
status generic_static_list_insert(generic_static_list *L,int *list_len,int index,element x) {
	element *ptr;
	int i;
	//移动index以及后面的数据，空出index位置,从最后开始移动
	if (*list_len >= MAX_LIST_LEN)return -1;//满了，不可以插入
	if (index<0 || index>*list_len)return -2;//插入位置非法
	ptr = (element *)L;//强转指针类型
	for (i = *list_len; i >index; i--) {
		ptr[i] = ptr[i-1];
	}
	//插入
	ptr[index] = x;
	(*list_len)++;
	return 0;
}
//静态表删除 在有list_len个元素的表L的index位置删除。index从0开始
status generic_static_list_delete(generic_static_list *L, int *list_len, int index) {
	int i;
	element *ptr;
	if (index<0 || index>(*list_len - 1))return -1;//删除位置错误
	ptr = (element *)L;//强转指针类型
	for (i = index+1; i < *list_len; i++) {//从删除点后一位开始，往回挪一位,避免删除尾部的特判
		ptr[i-1] = ptr[i];
	}
	(*list_len)--;//避免优先级问题
	return 0;
}


#endif
/********单链表部分**************/
#if ( DYNAMIC_LIST == 1)
/***********单链表私有变量********************/


/***********单链表全局变量********************/
status generic_dynamic_single_list_insert(generic_dynamic_single_list *L, generic_dynamic_single_list *node) {
	L->next = node;
	node->next = NULL;//尾插法
	return 0;
}
//L是头结点,删除链表中第一个值是e的结点
status generic_dynamic_single_list_delete(generic_dynamic_single_list *L,element e) {
	generic_dynamic_single_list *pre_node = L;
	generic_dynamic_single_list *cur_node = L->next;
	for (; cur_node != NULL; pre_node=cur_node,cur_node = cur_node->next) {
		if (cur_node->value == e) {
			pre_node->next = cur_node->next;
			free(cur_node);
			return 0;
		}
	}
	return -1;
}
#endif

