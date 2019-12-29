#include "user_usart.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#pragma import(__use_no_semihosting)   
/*
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
*/

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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA

	 //第4步：Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	
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
	NVIC_InitTypeDef NVIC_InitStructure;	
	//使能 USART2 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//打开串口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //打开PA端口时钟

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
	 //Usart2 NVIC 配置
	
	
	/*!!!!!!!!!!!!**attetion!!!****!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	
	
	//忘记配置中断线了，导致一直无法进入中断！！！！！！
	
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
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
	NVIC_InitTypeDef NVIC_InitStructure;
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
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化GPIOB
	 //Usart2 NVIC 配置
	
	
	/*!!!!!!!!!!!!**attetion!!!****!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	
	
	//忘记配置中断线了，导致一直无法进入中断！！！！！！
	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
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


#if defined(UART4)&&(ZET6==1)
//串口4初始化
void UART4_Config(int boaud_rate)   //初始化 配置USART4
{
	GPIO_InitTypeDef GPIO_InitStructure;	   //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	   //串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
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
	 //第4步：Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
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


#endif
#if defined(UART5)&&(ZET6==1)
//串口5初始化
void UART5_Config(int boaud_rate)   //初始化 配置UART5
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;		//串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
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
	   //第4步：Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	//配置UART5参数
	USART_InitStructure.USART_BaudRate = boaud_rate;	                    //波特率设置：2400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(UART5, &USART_InitStructure);                       //初始化UART5

	//打开发送中断和接收中断(如果需要中断)
	//USART_ITConfig(UART5, USART_IT_TXE, ENABLE);  // 使能指定的UART5发送中断
  	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); // 使能指定的UART5接收中断

	//使能 UART5， 配置完毕
	USART_Cmd(UART5, ENABLE);                             // UART5使能

	//如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(UART5, USART_FLAG_TC);                //清串口5发送标志
}


#endif
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


#ifdef USART1

static volatile struct{
	uint16_t	tri, twi, tct;
	uint16_t	rri, rwi, rct;
	uint8_t		tbuf[USART1_TXB];
	uint8_t		rbuf[USART1_RXB];
} Fifo1;

static int u1_putc(uchar c){
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
static uchar u1_getc(){
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
const volatile Usart_Obj_st usart1_obj={
	u1_putc,
	u1_getc,
};
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


#endif
#ifdef USART2

static volatile struct{
	uint16_t	tri, twi, tct;
	uint16_t	rri, rwi, rct;
	uint8_t		tbuf[USART2_TXB];
	uint8_t		rbuf[USART2_RXB];
} Fifo2;

static int u2_putc(uchar c){
	int i;
	/* Wait for tx fifo is not full */
	while (Fifo2.tct >= USART2_TXB) ;

	i = Fifo2.twi;		/* Put a byte into Tx fifo */
	Fifo2.tbuf[i] = c;
	Fifo2.twi = ++i % USART2_TXB;
	__disable_irq();
	Fifo2.tct++;
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//使能发送中断
	__enable_irq();
	return 0;
}
static uchar u2_getc(){
	uchar d;
	int i;

	/* Wait while rx fifo is empty */
	while (!Fifo2.rct);

	i = Fifo2.rri;			/* Get a byte from rx fifo */
	d = Fifo2.rbuf[i];
	Fifo2.rri = ++i % USART2_RXB;
	__disable_irq();
	Fifo2.rct--;
	__enable_irq();

	return d;
}
const volatile Usart_Obj_st usart2_obj={
	u2_putc,
	u2_getc,
};
void USART2_IRQHandler(){
	uchar d;
	uint i=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET){  //接收中断
		d = (uchar)USART_ReceiveData(USART2);//读取16bit的1字
		i = Fifo2.rct;
		if (i < USART2_RXB) {	/* Store it into the rx fifo if not full */
			Fifo2.rct = ++i;
			i = Fifo2.rwi;
			Fifo2.rbuf[i] = d;
			Fifo2.rwi = ++i % USART2_RXB;
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) == SET){  //发送中断,要求使能了发送中断
		i = Fifo2.tct;
		if (i--) {	/* There is any data in the tx fifo */
			Fifo2.tct = (uint16_t)i;
			i = Fifo2.tri;
			user_Usart_SendByte(USART2,Fifo2.tbuf[i]);
			Fifo2.tri = ++i % USART2_TXB;
		} else {	/* No data in the tx fifo */
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);//失能发送中断
			USART_ClearITPendingBit(USART2,USART_IT_TXE);//清除发送中断标志
		}
	}
}
#endif
#ifdef USART3
static volatile struct{
	uint16_t	tri, twi, tct;
	uint16_t	rri, rwi, rct;
	uint8_t		tbuf[USART3_TXB];
	uint8_t		rbuf[USART3_RXB];
} Fifo3;

static int u3_putc(uchar c){
	int i;
	/* Wait for tx fifo is not full */
	while (Fifo3.tct >= USART3_TXB) ;

	i = Fifo3.twi;		/* Put a byte into Tx fifo */
	Fifo3.tbuf[i] = c;
	Fifo3.twi = ++i % USART3_TXB;
	__disable_irq();
	Fifo3.tct++;
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//使能发送中断
	__enable_irq();
	return 0;
}
static uchar u3_getc(){
	uchar d;
	int i;

	/* Wait while rx fifo is empty */
	while (!Fifo3.rct);

	i = Fifo3.rri;			/* Get a byte from rx fifo */
	d = Fifo3.rbuf[i];
	Fifo3.rri = ++i % USART3_RXB;
	__disable_irq();
	Fifo3.rct--;
	__enable_irq();

	return d;
}
const volatile Usart_Obj_st usart3_obj={
	u3_putc,
	u3_getc,
};
void USART3_IRQHandler(){
	uchar d;
	uint i=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET){  //接收中断
		d = (uchar)USART_ReceiveData(USART3);//读取16bit的1字
		i = Fifo3.rct;
		if (i < USART3_RXB) {	/* Store it into the rx fifo if not full */
			Fifo3.rct = ++i;
			i = Fifo3.rwi;
			Fifo3.rbuf[i] = d;
			Fifo3.rwi = ++i % USART3_RXB;
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) == SET){  //发送中断,要求使能了发送中断
		i = Fifo3.tct;
		if (i--) {	/* There is any data in the tx fifo */
			Fifo3.tct = (uint16_t)i;
			i = Fifo3.tri;
			user_Usart_SendByte(USART3,Fifo3.tbuf[i]);
			Fifo3.tri = ++i % USART3_TXB;
		} else {	/* No data in the tx fifo */
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);//失能发送中断
			USART_ClearITPendingBit(USART3,USART_IT_TXE);//清除发送中断标志
		}
	}
}

#endif
//uart4 5 的处理有些特殊，不可以这样用
#if defined(UART4)&&(ZET6==1)
static volatile struct{
	uint16_t	tri, twi, tct;
	uint16_t	rri, rwi, rct;
	uint8_t		tbuf[UART4_TXB];
	uint8_t		rbuf[UART4_RXB];
} Fifo4;

static int u4_putc(uchar c){
	int i;
	/* Wait for tx fifo is not full */
	while (Fifo4.tct >= UART4_TXB) ;

	i = Fifo4.twi;		/* Put a byte into Tx fifo */
	Fifo4.tbuf[i] = c;
	Fifo4.twi = ++i % UART4_TXB;
	__disable_irq();
	Fifo4.tct++;
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);//使能发送中断
	__enable_irq();
	return 0;
}
static uchar u4_getc(){
	uchar d;
	int i;

	/* Wait while rx fifo is empty */
	while (!Fifo4.rct);

	i = Fifo4.rri;			/* Get a byte from rx fifo */
	d = Fifo4.rbuf[i];
	Fifo4.rri = ++i % UART4_RXB;
	__disable_irq();
	Fifo1.rct--;
	__enable_irq();

	return d;
}
const volatile Usart_Obj_st usart4_obj={
	u4_putc,
	u4_getc,
};

void UART4_IRQHandler(){
	uchar d;
	uint i=0;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET){  //接收中断
		d = (uchar)USART_ReceiveData(UART4);//读取16bit的1字
		i = Fifo4.rct;
		if (i < UART4_RXB) {	/* Store it into the rx fifo if not full */
			Fifo4.rct = ++i;
			i = Fifo4.rwi;
			Fifo4.rbuf[i] = d;
			Fifo4.rwi = ++i % UART4_RXB;
		}
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(UART4, USART_IT_TXE) == SET){  //发送中断,要求使能了发送中断
		i = Fifo4.tct;
		if (i--) {	/* There is any data in the tx fifo */
			Fifo1.tct = (uint16_t)i;
			i = Fifo4.tri;
			user_Usart_SendByte(UART4,Fifo4.tbuf[i]);
			Fifo4.tri = ++i % UART4_TXB;
		} else {	/* No data in the tx fifo */
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);//失能发送中断
			USART_ClearITPendingBit(UART4,USART_IT_TXE);//清除发送中断标志
		}
	}
}

#endif

#if defined(UART5)&&(ZET6==1)
static volatile struct{
	uint16_t	tri, twi, tct;
	uint16_t	rri, rwi, rct;
	uint8_t		tbuf[UART5_TXB];
	uint8_t		rbuf[UART5_RXB];
} Fifo5;

static int u5_putc(uchar c){
	int i;
	/* Wait for tx fifo is not full */
	while (Fifo5.tct >= UART5_TXB) ;

	i = Fifo5.twi;		/* Put a byte into Tx fifo */
	Fifo5.tbuf[i] = c;
	Fifo5.twi = ++i % UART5_TXB;
	__disable_irq();
	Fifo5.tct++;
	USART_ITConfig(UART5, USART_IT_TXE, ENABLE);//使能发送中断
	__enable_irq();
	return 0;
}
static uchar u5_getc(){
	uchar d;
	int i;

	/* Wait while rx fifo is empty */
	while (!Fifo5.rct);

	i = Fifo5.rri;			/* Get a byte from rx fifo */
	d = Fifo5.rbuf[i];
	Fifo5.rri = ++i % UART5_RXB;
	__disable_irq();
	Fifo1.rct--;
	__enable_irq();

	return d;
}
const volatile Usart_Obj_st usart5_obj={
	u5_putc,
	u5_getc,
};

void UART5_IRQHandler(){
	uchar d;
	uint i=0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET){  //接收中断
		d = (uchar)USART_ReceiveData(UART5);//读取16bit的1字
		i = Fifo5.rct;
		if (i < UART5_RXB) {	/* Store it into the rx fifo if not full */
			Fifo5.rct = ++i;
			i = Fifo5.rwi;
			Fifo5.rbuf[i] = d;
			Fifo5.rwi = ++i % UART5_RXB;
		}
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//清除接收中断标志
	}
	
	if(USART_GetITStatus(UART5, USART_IT_TXE) == SET){  //发送中断,要求使能了发送中断
		i = Fifo5.tct;
		if (i--) {	/* There is any data in the tx fifo */
			Fifo1.tct = (uint16_t)i;
			i = Fifo5.tri;
			user_Usart_SendByte(UART5,Fifo5.tbuf[i]);
			Fifo5.tri = ++i % UART5_TXB;
		} else {	/* No data in the tx fifo */
			USART_ITConfig(UART5, USART_IT_TXE, DISABLE);//失能发送中断
			USART_ClearITPendingBit(UART5,USART_IT_TXE);//清除发送中断标志
		}
	}
}

#endif




