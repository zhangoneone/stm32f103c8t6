#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "user_tcpip_control.h"
#include "api.h"
xTaskHandle Tcp_Server_Task_TCB;
xTaskHandle Tcp_Handle_Task_TCB;
struct netconn *newnetc;
void tcp_handle(){
	int counter=0;
	struct netbuf *buf; // 申明指针
	netconn_recv(newnetc,&buf);
	xprintf_s("tcp get:%s\r\n",buf);
	netconn_write(newnetc,"hello!",7,NETCONN_DONTBLOCK);
}
void tcp_server(){
	struct netbuf *buf; // 申明指针
	struct netconn *netc;
	ip4_addr_t ipaddr;
	netc = netconn_new(NETCONN_TCP);
	IP4_ADDR(&ipaddr, 192, 168, 1, 130);
	netconn_bind(netc,&ipaddr,8080);
	netconn_listen(netc);
	while(1){
		if(netconn_accept(netc,&newnetc)==ERR_OK){
			xTaskCreate(tcp_handle,"tcp_handle_task",128,NULL,3,&Tcp_Handle_Task_TCB );
		}		
	}
}
