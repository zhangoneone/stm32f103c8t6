#include "ns_queue.h"
/********静态队列部分**************/

#if(STATIC_QUEUE == 1)

/***********静态队列私有变量********************/
////这里初始状态应该为head=tail=-1
//判空是head==tail
//判满是tail = MAX-1
static status generic_static_queue_empty(generic_static_queue *q) {
	if (q->queue_head == q->queue_tail)return true;
	else return false;
}
static status generic_static_queue_full(generic_static_queue *q) {
	if (q->queue_tail == MAX_QUEUE_LEN - 1)return true;
	else return false;
}
/***********静态队列公有变量********************/
//入队
status generic_static_queue_en(generic_static_queue *q,element x) {
	element *ptr = (element *)q->load;
	if (generic_static_queue_full(q))return -1;
	ptr[q->queue_tail++] = x;
	return 0;
}
//出队
status generic_static_queue_de(generic_static_queue *q, element *x) {
	element *ptr = (element *)q->load;
	if (generic_static_queue_empty(q))return -1;
	//先后移再取数据
	*x = ptr[++q->queue_head];
	return 0;

}
#endif
/***********静态循环队列私有变量********************/

#if(STATIC_CYCLE_QUEUE == 1)
//静态循环队列，采取舍弃1个空间的方式。
//初始状态是tail和head均为0，因为舍弃掉1个空间，可以先加，再赋值。舍弃掉第一个空间
//判满是tail在head后面1位
//判空是tail等于head
static status generic_static_cycle_queue_empty(generic_static_cycle_queue *q) {
	if (q->queue_head == q->queue_tail)return true;
	else return false;
}
static status generic_static_cycle_queue_full(generic_static_cycle_queue *q) {
	if ((q->queue_tail +1)% MAX_QUEUE_LEN == q->queue_head)return true;//tail+1并取模，试探是否等于head，等于则满了
	else return false;
}
/***********静态循环队列公有变量********************/
//入队
status generic_static_cycle_queue_en(generic_static_cycle_queue *q, element x) {
	element *ptr = (element *)q->load;
	if (generic_static_cycle_queue_full(q))return -1;
	//因为是循环队列，所以先处理tail的位置
	q->queue_tail = (q->queue_tail + 1) % MAX_QUEUE_LEN;
	ptr[q->queue_tail] = x;
	return 0;
}
//出队
status generic_static_cycle_queue_de(generic_static_cycle_queue *q, element *x) {
	element *ptr = (element *)q->load;
	if (generic_static_cycle_queue_empty(q))return -1;
	//因为是循环队列，所以先处理head的位置
	q->queue_head = (q->queue_head + 1) % MAX_QUEUE_LEN;
	//取数据
	*x = ptr[q->queue_head];
	return 0;
}

#endif
/***********静态双向队列私有变量********************/
//初始状态应该是m = (MAX_QUEUE_LEN -1)/2
//left = m+1,right = m
/*
static status generic_static_double_queue_empty(generic_static_double_queue *q) {
	if (q->queue_head == q->queue_tail)return true;
	else return false;
}
static status generic_static_double_queue_full(generic_static_double_queue *q) {
	if ((q->queue_tail + 1) % MAX_QUEUE_LEN == q->queue_head)return true;//tail+1并取模，试探是否等于head，等于则满了
	else return false;
}
*/
/***********静态双向队列公有变量********************/