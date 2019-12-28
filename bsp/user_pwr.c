#include "user_pwr.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
/*stm32单独拎出来的可编程电源管理，主要是睡眠、待机、停机三种省电模式
还有DEBGUG模式，降低系统时钟等
另外还有ADC单独供电等细节问题
*/

uchar user_pwr_mode_matrix[PWR_MODE_MAX];//标志stm32当前状态的，某个置1则在该模式下

USER_RETURN_VALUE user_set_rtc_backup(FunctionalState NewState){
	 PWR_BackupAccessCmd(NewState);
	 return 0;
}

//退出待机模式的方法
//PA0上升沿
//rtc闹钟中断上升沿
//独立看门狗复位
//复位引脚

//待机模式,PA0上升沿退出待机模式
USER_RETURN_VALUE user_standby_mode(){
	GPIO_InitTypeDef  GPIO_InitStructure;  		  
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//GPIOA复用时钟打开
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
  PWR_WakeUpPinCmd(ENABLE);//睡眠唤醒引脚使能
	PWR_EnterSTANDBYMode();
}
//停机模式
USER_RETURN_VALUE user_stop_mode(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//任一中断都可唤醒，低功耗模式
}
void reboot(){
		__set_FAULTMASK(1); // 关闭一切中断
    NVIC_SystemReset(); // 复位
}
//获取当前的电源管理状态
//把状态数组的当前状态置位
USER_RETURN_VALUE user_get_current_status(uchar *matrix){
	 return 0;
}