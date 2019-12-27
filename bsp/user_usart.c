#include "user_usart.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

};

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
} 

//串口1初始化
void USART1_Config(int boaud_rate)		//初始化 配置USART1
{
	GPIO_InitTypeDef     GPIO_InitStructure;   //串口端口配置结构体变量
	USART_InitTypeDef    USART_InitStructure;  //串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//第1步：打开GPIO和USART部件的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	 //打开GPIOA时钟和GPIOA复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//打开串口复用时钟
	USART_DeInit(USART1);  //复位串口1

	//第2步：将USART1 Tx（发送脚）的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //串口1发送脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA
														  
	//第3步：将USART Rx（接收脚）的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   //串口1接收脚
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA

//	 //第4步：Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//第5步：配置USART1参数
	USART_InitStructure.USART_BaudRate             = boaud_rate;							 //波特率设置：115200
	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;			 //数据位数设置：8位
	USART_InitStructure.USART_StopBits             = USART_StopBits_1;				 //停止位设置：1位
	USART_InitStructure.USART_Parity               = USART_Parity_No;				 //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	 //接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);										 //初始化USART1

    //打开发送中断和接收中断(如果需要中断)
	//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // 使能指定的USART1发送中断 ；此处使能导致不停进入串口中断
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能指定的USART1接收中断 ；

	//第6步：使能 USART1， 配置完毕
	 USART_Cmd(USART1, ENABLE);							   //使能 USART1

    //如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(USART1, USART_FLAG_TC);                //清串口1发送标志
	
}

//串口2初始化
void USART2_Config(int boaud_rate)   //初始化 配置USART2
{
	GPIO_InitTypeDef    GPIO_InitStructure;	   //串口端口配置结构体变量
	USART_InitTypeDef   USART_InitStructure;   //串口参数配置结构体变量

	//使能 USART2 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   //打开PA端口时钟

	//将USART2 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //选定哪个IO口 现选定PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //初始化GPIOA

	//将USART2 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //选定哪个IO口 现选定PA3
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART2参数
	USART_InitStructure.USART_BaudRate = boaud_rate;	                    //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART2, &USART_InitStructure);                       //初始化USART2
	
	//打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  // 使能指定的USART2发送中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能指定的USART2接收中断

	//使能 USART2， 配置完毕
	USART_Cmd(USART2, ENABLE);                             // USART2使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(USART2, USART_FLAG_TC);                //清串口2发送标志
}

//串口3初始化
void USART3_Config(int boaud_rate)   //初始化 配置USART3
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	  //串口参数配置结构体变量

	//使能 USART3 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //打开PB端口时钟
  	
	//将USART3 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //选定哪个IO口 现选定PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //初始化GPIOB
	
	//将USART3 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //选定哪个IO口 现选定PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART3参数
	USART_InitStructure.USART_BaudRate = boaud_rate;	                    //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART3, &USART_InitStructure);                       //初始化USART3
	
    //打开发送中断和接收中断(如果需要中断)
	//  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // 使能指定的USART3发送中断
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 使能指定的USART3接收中断

	//使能 USART3， 配置完毕
	USART_Cmd(USART3, ENABLE);                             // USART3使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(USART3, USART_FLAG_TC);                //清串口3发送标志
}

//串口4初始化
void UART4_Config(int boaud_rate)   //初始化 配置USART4
{
	GPIO_InitTypeDef GPIO_InitStructure;	   //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	   //串口参数配置结构体变量

	//使能 UART4 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //打开PC端口时钟
	
	//将USART4 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //选定哪个IO口 现选定PC10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);    				  //初始化GPIOC

	//将UART4 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //选定哪个IO口 现选定PC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);                    //初始化GPIOC
	  
	//配置UART4参数
	USART_InitStructure.USART_BaudRate = boaud_rate;	                    //波特率设置：9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(UART4, &USART_InitStructure);                       //初始化UART4
	
	//打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(UART4, USART_IT_TXE, ENABLE);  // 使能指定的UART4发送中断
  	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); // 使能指定的UART4接收中断

	//使能 UART4， 配置完毕
	USART_Cmd(UART4, ENABLE);                             // UART4使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(UART4, USART_FLAG_TC);                //清串口4发送标志
}

//串口5初始化
void UART5_Config(int boaud_rate)   //初始化 配置UART5
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;		//串口参数配置结构体变量

	//使能 UART5 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//打开串口复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //打开PC端口时钟
	
	//将UART5 Tx（发送脚）的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				  //选定哪个IO口 现选定PC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);    				  //初始化GPIOC

	//将UART5 Rx（接收脚）的GPIO配置为浮空输入模式															  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //选定哪个IO口 现选定PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);                    //初始化GPIOC
	  
	//配置UART5参数
	USART_InitStructure.USART_BaudRate = boaud_rate;	                    //波特率设置：2400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(UART5, &USART_InitStructure);                       //初始化UART5

	//打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(UART5, USART_IT_TXE, ENABLE);  // 使能指定的UART4发送中断
  	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); // 使能指定的UART4接收中断

	//使能 UART5， 配置完毕
	USART_Cmd(UART5, ENABLE);                             // UART5使能

	//如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(UART5, USART_FLAG_TC);                //清串口5发送标志
}

void user_Usart_SendByte(USART_TypeDef * Usart_Num,uchar SendData)	  //用串口usart_num发送一个字节数据
{	  
	USART_SendData(Usart_Num,SendData);   //用串口发送一个字节
	while(USART_GetFlagStatus(Usart_Num, USART_FLAG_TXE) == RESET);  //等待串口发送完毕
} 

int user_Usart_Send(USART_TypeDef * Usart_Num,const uchar *str)	  //用串口usart_num发送字符串
{	
	int write_counter = 0;
	while(*str!=NULL){
		USART_SendData(Usart_Num,*str);   //用串口发送一个字节
		while(USART_GetFlagStatus(Usart_Num, USART_FLAG_TXE) == RESET);  //等待串口发送完毕
		write_counter++;
		str++;
	}
	return write_counter;
}

uchar* u_gets(USART_TypeDef * Usart_Num){
	return 0;
}
int u_puts(USART_TypeDef * Usart_Num,uchar *s){
	return 0;
}

#ifdef USART1

static volatile struct{
	uint16_t	tri, twi, tct;
	uint16_t	rri, rwi, rct;
	uint8_t		tbuf[USART1_TXB];
	uint8_t		rbuf[USART1_RXB];
} Fifo1;

int u1_putc(uchar c){
	int i;
	/* Wait for tx fifo is not full */
	while (Fifo1.tct >= USART1_TXB) ;

	i = Fifo1.twi;		/* Put a byte into Tx fifo */
	Fifo1.tbuf[i] = c;
	Fifo1.twi = ++i % USART1_TXB;
	__disable_irq();
	Fifo1.tct++;
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//使能发送中断
	__enable_irq();
	return 0;
}
uchar u1_getc(){
	uchar d;
	int i;

	/* Wait while rx fifo is empty */
	while (!Fifo1.rct);

	i = Fifo1.rri;			/* Get a byte from rx fifo */
	d = Fifo1.rbuf[i];
	Fifo1.rri = ++i % USART1_RXB;
	__disable_irq();
	Fifo1.rct--;
	__enable_irq();

	return d;
}

void USART1_IRQHandler(){
	uchar d;
	uint i=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){  //接收中断
		d = (uchar)USART_ReceiveData(USART1);//读取16bit的1字
		i = Fifo1.rct;
		if (i < USART1_RXB) {	/* Store it into the rx fifo if not full */
			Fifo1.rct = ++i;
			i = Fifo1.rwi;
			Fifo1.rbuf[i] = d;
			Fifo1.rwi = ++i % USART1_RXB;
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) == SET){  //发送中断,要求使能了发送中断
		i = Fifo1.tct;
		if (i--) {	/* There is any data in the tx fifo */
			Fifo1.tct = (uint16_t)i;
			i = Fifo1.tri;
			user_Usart_SendByte(USART1,Fifo1.tbuf[i]);
			Fifo1.tri = ++i % USART1_TXB;
		} else {	/* No data in the tx fifo */
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//失能发送中断
			USART_ClearITPendingBit(USART1,USART_IT_TXE);//清除发送中断标志
		}
	}
}

const volatile usart_obj usart1_obj={
	u1_putc,
	u1_getc,
	u_gets,
	u_puts,
};

#endif
#ifdef USART2
void USART2_IRQHandler(){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == RESET){  //接收中断
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) == RESET){  //发送中断,要求使能了发送中断
		
		USART_ClearITPendingBit(USART2,USART_IT_TXE);//清除发送中断标志
	}
}
#endif
#ifdef USART3
void USART3_IRQHandler(){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == RESET){  //接收中断
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) == RESET){  //发送中断,要求使能了发送中断
		
		USART_ClearITPendingBit(USART3,USART_IT_TXE);//清除发送中断标志
	}
}

#endif
//uart4 5 的处理有些特殊，不可以这样用
void USART4_IRQHandler(){
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == RESET){  //接收中断
		
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(UART4, USART_IT_TXE) == RESET){  //发送中断,要求使能了发送中断
		
		USART_ClearITPendingBit(UART4,USART_IT_TXE);//清除发送中断标志
	}
}
void USART5_IRQHandler(){
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == RESET){  //接收中断
		
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(UART5, USART_IT_TXE) == RESET){  //发送中断,要求使能了发送中断
		
		USART_ClearITPendingBit(UART5,USART_IT_TXE);//清除发送中断标志
	}
}




