#include "user_exti.h"
/*
stm32有19个exit，其中前16个连到了GPIO上，通过选择，可以覆盖所有的GPIO
还有3个连到了rtc闹钟，usb，网络等设备
另外，exit可以选择将信号发送给NVIC或者是脉冲发生器。
前者会产生cortex-m3内核中断，后者会产生脉冲信号，脉冲信号可以连接到其他的外设。
即中断和事件的区别。
*/

/*事件机制
线类型选择位事件线，该线的中断屏蔽开启，事件屏蔽关闭。即可产生事件
事件的脉冲发生器，连接到了片内其他外设
*/

/*exit初始化*/
/*
AFIO和GPIO时钟使能
GPIO初始化
EXIT线设置
中断处理函数
*/

static struct{
	GPIO_TypeDef* GPIOx;//注册的gpio组
	uint EXTI_Line;//注册的线
	uint Pin;
	uint GPIO_PortSourceGPIOx;
	uint GPIO_PinSourcex;
	EXTITrigger_TypeDef Trigger;
	uint EXTIx_IRQn;
	void(*callback)();//注册的中断回调函数组
}regist_exti_info[MAX_EXTI_NUM];

//找该gpio是否注册过，没有则返回-1，否则返回下标
//EXITx只能被ABCDEFGHI中某个GPIO注册。即一根线不能被多次注册
static int find_exti(uint EXTI_Line){
	int i =0;
	for(;i<MAX_EXTI_NUM;i++){
		if(regist_exti_info[i].EXTI_Line==EXTI_Line)return i;
	}
	if(i == MAX_EXTI_NUM)return -1;
}
//找空余的地儿放下注册结构，没有则返回-1
static int find_empty_site(){
	int i =0;
	for(;i<MAX_EXTI_NUM;i++){
		if(regist_exti_info[i].EXTI_Line==0)return i;
	}
	if(i == MAX_EXTI_NUM)return -1;
}

static int exti_init(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn,void(*cb)()){
	int i = -1;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	//GPIO时钟开启
	switch((uint)GPIOx){
		case (uint)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
		case (uint)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case (uint)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
		case (uint)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);break;
		case (uint)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);break;
		case (uint)GPIOF:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);break;
		case (uint)GPIOG:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);break;
		default:break;
	}
	//1.使用外部中断时，AFIO时钟必须使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2.GPIO配置
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//
  EXTI_Init(&EXTI_InitStructure);//
  //4.NVIC配置
  NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	//初始化注册结构表
	i = find_exti(EXTI_Linex);//如果已经存在了，则更新结构表
	if(i == -1)
		i = find_empty_site();//如果不存在，则新建
	if(i!= -1){
		regist_exti_info[i].callback = cb;
		regist_exti_info[i].EXTI_Line = EXTI_Linex;
		regist_exti_info[i].GPIOx = GPIOx;
		regist_exti_info[i].EXTIx_IRQn = EXTIx_IRQn;
		regist_exti_info[i].GPIO_PinSourcex = GPIO_PinSourcex;
		regist_exti_info[i].GPIO_PortSourceGPIOx = GPIO_PortSourceGPIOx;
		regist_exti_info[i].Pin = Pin;
		regist_exti_info[i].Trigger = Trigger;
		EXTI->IMR &= ~(EXTI_Linex); //默认关闭的
		EXTI->EMR &= ~(EXTI_Linex); //默认关闭的
		return 0;
	}
	else return -1;
}

static void exti_start(uint EXTI_Line){
	//1.使用外部中断时，AFIO时钟必须使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	EXTI->IMR |= EXTI_Line;//开启中断
	EXTI->EMR &= ~(EXTI_Line); //关闭事件
}

static void exti_stop(uint EXTI_Line){
	EXTI->IMR &= ~(EXTI_Line); //关闭中断
	EXTI->EMR &= ~(EXTI_Line); //关闭事件
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
}


static void exti_trigger(uint EXTI_Line){
	EXTI_GenerateSWInterrupt(EXTI_Line);
}

static int exti_deinit(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn
){
	int i =-1;
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  //2.GPIO配置
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO???50MHz
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //?EXIT?9???PB9
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //???????
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;//?????
  EXTI_Init(&EXTI_InitStructure);//?????
	//找到注册表，销毁
	i = find_exti(EXTI_Linex);
	if(i!= -1){
		regist_exti_info[i].callback = 0;
		regist_exti_info[i].EXTI_Line = 0;
		regist_exti_info[i].GPIOx = 0;
		regist_exti_info[i].EXTIx_IRQn = 0;
		regist_exti_info[i].GPIO_PinSourcex = 0;
		regist_exti_info[i].GPIO_PortSourceGPIOx = 0;
		regist_exti_info[i].Pin = 0;
		regist_exti_info[i].Trigger = 0;
		EXTI->IMR &= ~(EXTI_Linex); //关闭中断
		EXTI->EMR &= ~(EXTI_Linex); //关闭事件
		return 0;
	}
	else return -1;
}

void EXTI0_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line0) != RESET){  //EXTI0中断
		 i = find_exti(EXTI_Line0);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line0);//清除标志位
		 }
	}
}
void EXTI1_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line1) != RESET){  //EXTI1中断
		 i = find_exti(EXTI_Line1);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line1);//清除标志位
		 }
	}	
}
void EXTI2_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line2) != RESET){  //EXTI2中断
		 i = find_exti(EXTI_Line2);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line2);//清除标志位
		 }
	}	
}
void EXTI3_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line3) != RESET){  //EXTI3中断
		 i = find_exti(EXTI_Line3);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line3);//清除标志位
		 }
	}	
}
void EXTI4_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line4) != RESET){  //EXTI4中断
		 i = find_exti(EXTI_Line4);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line4);//清除标志位
		 }
	}	
}
/*
void EXTI9_5_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line5) != RESET){  //EXTI5中断
		 i = find_exti(EXTI_Line5);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line5);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line6) != RESET){  //EXTI6中断
		 i = find_exti(EXTI_Line6);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line6);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line7) != RESET){  //EXTI7中断
		 i = find_exti(EXTI_Line7);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line7);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line8) != RESET){  //EXTI8中断
		 i = find_exti(EXTI_Line8);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line8);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line9) != RESET){  //EXTI9中断
		 i = find_exti(EXTI_Line9);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line9);//清除标志位
		 }
	}
}
*/
void EXTI15_10_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line10) != RESET){  //EXTI10中断
		 i = find_exti(EXTI_Line10);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line10);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line11) != RESET){  //EXTI11中断
		 i = find_exti(EXTI_Line11);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line11);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line12) != RESET){  //EXTI12中断
		 i = find_exti(EXTI_Line12);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line12);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line13) != RESET){  //EXTI13中断
		 i = find_exti(EXTI_Line13);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line13);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line14) != RESET){  //EXTI14中断
		 i = find_exti(EXTI_Line14);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line14);//清除标志位
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line15) != RESET){  //EXTI15中断
		 i = find_exti(EXTI_Line15);
		 if(i != -1){
			regist_exti_info[i].callback();//执行回调
			EXTI_ClearITPendingBit(EXTI_Line15);//清除标志位
		 }
	}	
}


const Exti_St exti_obj={
	exti_init,
	exti_start,
	exti_stop,
	exti_trigger,
	exti_deinit,
};



static int event_init(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn){
	int i = -1;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	//GPIO时钟开启
	switch((uint)GPIOx){
		case (uint)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
		case (uint)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case (uint)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
		case (uint)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);break;
		case (uint)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);break;
		case (uint)GPIOF:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);break;
		case (uint)GPIOG:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);break;
		default:break;
	}
	//1.使用外部中断时，AFIO时钟必须使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2.GPIO配置
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//
  EXTI_Init(&EXTI_InitStructure);//
	//初始化注册结构表
	i = find_exti(EXTI_Linex);//如果已经存在了，则更新结构表
	if(i == -1)
		i = find_empty_site();//如果不存在，则新建
	if(i!= -1){
		regist_exti_info[i].callback = 0;
		regist_exti_info[i].EXTI_Line = EXTI_Linex;
		regist_exti_info[i].GPIOx = GPIOx;
		regist_exti_info[i].EXTIx_IRQn = EXTIx_IRQn;
		regist_exti_info[i].GPIO_PinSourcex = GPIO_PinSourcex;
		regist_exti_info[i].GPIO_PortSourceGPIOx = GPIO_PortSourceGPIOx;
		regist_exti_info[i].Pin = Pin;
		regist_exti_info[i].Trigger = Trigger;
		EXTI->IMR &= ~(EXTI_Linex); //关闭中断
		EXTI->EMR &= ~(EXTI_Linex); //关闭事件
		return 0;
	}
	else return -1;
}



static void event_start(uint EXTI_Line){
	//1.使用外部中断时，AFIO时钟必须使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	EXTI->IMR &= ~(EXTI_Line); //关闭中断
	EXTI->EMR |= EXTI_Line; //开启事件
}

static void event_stop(uint EXTI_Line){
	EXTI->IMR &= ~(EXTI_Line); //关闭中断
	EXTI->EMR &= ~(EXTI_Line); //关闭事件
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
}
static void event_trigger(uint EXTI_Line){
	EXTI_GenerateSWInterrupt(EXTI_Line);
}
static int event_deinit(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn
){
	int i =-1;
	GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  //2.GPIO配置
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO???50MHz
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //?EXIT?9???PB9
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //???????
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;//?????
  EXTI_Init(&EXTI_InitStructure);//?????
	//找到注册表，销毁
	i = find_exti(EXTI_Linex);
	if(i!= -1){
		regist_exti_info[i].callback = 0;
		regist_exti_info[i].EXTI_Line = 0;
		regist_exti_info[i].GPIOx = 0;
		regist_exti_info[i].EXTIx_IRQn = 0;
		regist_exti_info[i].GPIO_PinSourcex = 0;
		regist_exti_info[i].GPIO_PortSourceGPIOx = 0;
		regist_exti_info[i].Pin = 0;
		regist_exti_info[i].Trigger = 0;
		EXTI->IMR &= ~(EXTI_Linex); //关闭中断
		EXTI->EMR &= ~(EXTI_Linex); //关闭事件
		return 0;
	}
	else return -1;
}


const Event_St event_obj={
	event_init,
	event_start,
	event_stop,
	event_trigger,
	event_deinit,
};
