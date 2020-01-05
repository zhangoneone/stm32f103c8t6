#include "user_spi.h"
#include "stm32f10x_spi.h"
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI1的初始化
/*********attetion！！！！！！！！************************/
//如果轮询标志位，就不要开启中断，并且清除标志位，这会导致SPI1_ReadWriteByte读不到标志位
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData){		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

SPI_InitTypeDef  SPI_InitStructure1;
void SPI1_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure1.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure1.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure1.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure1.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure1.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure1.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure1.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure1);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	//NVIC配置
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 //中断配置
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE|SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输		 
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
  
void SPI1_SetSpeed(u8 SpeedSet){
	SPI_InitStructure1.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI_InitStructure1);
	SPI_Cmd(SPI1,ENABLE);
} 

void SPI1_IRQHandler(){
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==SET){//接收缓冲区不空，可以收数据
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_RXNE);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==SET){//发送缓冲区为空，可以发数据
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_TXE);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_OVR)==SET){
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_OVR);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_MODF)==SET){
		SPI_I2S_ClearFlag(SPI1,SPI_FLAG_MODF);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_CRCERR)==SET){
		SPI_I2S_ClearFlag(SPI1,SPI_FLAG_CRCERR);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,I2S_FLAG_UDR)==SET){
		SPI_I2S_ClearFlag(SPI1,I2S_FLAG_UDR);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,I2S_FLAG_CHSIDE)==SET){
		SPI_I2S_ClearFlag(SPI1,I2S_FLAG_CHSIDE);
	}
}

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//这里针是对SPI2的初始化


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData){		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}


void SPI2_Init(void){
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
// //NVIC配置
//  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	 //中断配置
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE|SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输			 
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler){
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 
} 


/*
void SPI2_IRQHandler(){
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET){//接收缓冲区不空，可以收数据
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_RXNE);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==SET){//发送缓冲区为空，可以发数据
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_TXE);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_OVR)==SET){
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_OVR);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_MODF)==SET){
		SPI_I2S_ClearFlag(SPI2,SPI_FLAG_MODF);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_CRCERR)==SET){
		SPI_I2S_ClearFlag(SPI2,SPI_FLAG_CRCERR);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,I2S_FLAG_UDR)==SET){
		SPI_I2S_ClearFlag(SPI2,I2S_FLAG_UDR);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,I2S_FLAG_CHSIDE)==SET){
		SPI_I2S_ClearFlag(SPI2,I2S_FLAG_CHSIDE);
	}
}
*/
