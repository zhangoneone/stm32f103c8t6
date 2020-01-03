#include "user_timer.h"

/*基本定时器区*/
#if defined(TIM6)&&defined(TIM7)
/**********************************************************
** 函数名: TIM6_Int_Init
** 功能描述:  基本定时器配置
** 输入参数: 无
** 输出参数: 无
** 说明：定时时间=(预分频数+1）*（计数值+1) /TIM6时钟(72MHz)，单位(s)
   这里溢出时间t=(7200*10000)/72000000s=1s
***********************************************************/
static void TIM6_Int_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //使能TIM6时钟
	
	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);  //根据指定的参数初始化TIMx的时间基数单位
	TIM_ARRPreloadConfig(TIM6,ENABLE);//使能重装寄存器的缓冲功能,此时更新事件在此次计数溢出才会生效
	//允许更新中断，触发方式中断
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);     //使能指定的TIM6中断,允许更新中断
	TIM_UpdateRequestConfig(TIM6,TIM_UpdateSource_Global);//计数器溢出时产生中断，设置更新事件不产生中断
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);

}

volatile char tim6_int_flag = 0;
/**********************************************************
** 函数名: TIM6_IRQHandler
** 功能描述: 定时器6的中断服务程序
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void TIM6_IRQHandler(void){
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET){
		//do your task
		tim6_int_flag++;
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//清除更新中断标志位
	}
}
/**********************************************************
** 函数名: TIM7_Int_Init
** 功能描述:  基本定时器配置
** 输入参数: 无
** 输出参数: 无
** 说明：定时时间=(预分频数+1）*（计数值+1) /TIM6时钟(72MHz)，单位(s)
   这里溢出时间t=(7200*10000)/72000000s=1s
***********************************************************/
static void TIM7_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //使能TIM7时钟
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);  //根据指定的参数初始化TIMx的时间基数单位
	TIM_ARRPreloadConfig(TIM7,ENABLE);//使能重装寄存器的缓冲功能,此时更新事件在此次计数溢出才会生效
	//允许更新中断，触发方式中断
	TIM_ITConfig(TIM7,TIM_IT_Update, ENABLE);     //使能指定的TIM7中断,允许更新中断
	TIM_UpdateRequestConfig(TIM7,TIM_UpdateSource_Global);//计数器溢出时产生中断，设置更新事件不产生中断
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);
	
}
volatile char tim7_int_flag = 0;
/**********************************************************
** 函数名: TIM7_IRQHandler
** 功能描述: 定时器7的中断服务程序
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void TIM7_IRQHandler(void){
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		//do your task
		tim7_int_flag++;
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除更新中断标志位
	}
}
/**********************************************************
** 函数名: Update
** 功能描述:  基本定时器更新
** 输入参数: 无
** 输出参数: 无
** 说明：定时时间=(预分频数+1）*（计数值+1) /TIM6时钟(72MHz)，单位(s)
   这里溢出时间t=(7200*10000)/72000000s=1s
***********************************************************/
static void update(TIM_TypeDef *TIMx,u16 arr,u16 psc,Update_option_t option){
	//禁止产生更新事件
	TIM_UpdateDisableConfig(TIMx,ENABLE);
	/* Set the Autoreload value */
  TIMx->ARR = arr;
 
  /* Set the Prescaler value */
  TIMx->PSC = psc;
	//允许产生更新事件
	TIM_UpdateDisableConfig(TIMx,DISABLE);
	if(option == immediately)		//强制更新，立即生效配置
		TIM_GenerateEvent(TIMx,TIM_EventSource_Update);
}
static void start(TIM_TypeDef *TIMx){	
	TIM_Cmd(TIMx, ENABLE);     //使能定时器
}
static void stop(TIM_TypeDef *TIMx){
	TIM_Cmd(TIMx, DISABLE);     //失能定时器
}
static void deinit(TIM_TypeDef *TIMx){
	TIM_DeInit(TIMx);
}
const Basic_Timer_t tim6 = {
	TIM6_Int_Init,
	update,
	start,
	stop,
	deinit,
};
const Basic_Timer_t tim7 = {
	TIM7_Int_Init,
	update,
	start,
	stop,
	deinit,
};

#endif