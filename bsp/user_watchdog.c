#include "user_watchdog.h" 
#include "stm32f10x_wwdg.h"
#if(IWDG == 1)
//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
	IWDG_SetPrescaler(prer);  //设置IWDG预分频值:设置IWDG预分频值为64
	IWDG_SetReload(rlr);  //设置IWDG重装载值
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
}

void IWDG_Start(){
	IWDG_Enable();  //使能IWDG
}
//喂独立看门狗
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();//reload										   
}
void IWDG_Init_to_1s(){
	IWDG_Init(4,625);    //预分频数为2^4x4=64,重载值为625,溢出时间为1s	
}	
#endif

#if(WWDG == 1)
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占2，子优先级3，组2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //抢占2，子优先级3，组2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC初始化
}
//窗口看门狗使用APB1作为时钟源，一般是36MHZ
//保存WWDG计数器的设置值,默认为最大. 
static u8 WWDG_CNT = 0x7f;
//初始化窗口看门狗 	
//tr   :T[6:0],计数器值 
//wr   :W[6:0],窗口值 
//fprer:分频系数（WDGTB）,仅最低2位有效 
//时基计算Fwwdg=PCLK1/(4096*2^fprer). 

void WWDG_Init(u8 tr,u8 wr,u32 fprer){ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDG时钟使能
	WWDG_CNT=tr&WWDG_CNT;   //初始化WWDG_CNT.   
	WWDG_SetPrescaler(fprer);////设置IWDG预分频值
	WWDG_SetWindowValue(wr);//设置窗口值
	WWDG_Enable(WWDG_CNT);	 //使能看门狗 ,	设置 counter .                  
	WWDG_ClearFlag();//清除提前唤醒中断标志位 
	WWDG_NVIC_Init();//初始化窗口看门狗 NVIC
} 
void WWDG_Start(){
	WWDG_EnableIT(); //开启窗口看门狗中断
}
//重设置WWDG计数器的值
void WWDG_Set_Counter(u8 cnt){
    WWDG_Enable(cnt);//使能看门狗,设置 counter .	 
}

//窗口看门狗中断服务程序
void WWDG_IRQHandler(void){
	WWDG_ClearFlag();
  while(WWDG_GetFlagStatus() == SET); // 等待中断标志位真正清除, 防止中断重入
	WWDG_SetCounter(WWDG_CNT);	  //当禁掉此句后,窗口看门狗将产生复位
}	
#endif
