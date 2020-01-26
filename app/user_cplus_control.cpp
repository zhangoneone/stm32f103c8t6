#include "user_cplus_control.hpp"

extern "C"{ void xprintf_s(const char*	fmt,...);}
void f_test::fun1(int a){
	t1=a;
}

void f_test::proxy_fun(){
	this->fun1(5);
}
extern "C"{
void do_action(){
	f_test *f = new f_test();
	f->proxy_fun();
	xprintf_s("cpp test:%d,success\n",f->t1);
}
}