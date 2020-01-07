#ifndef NS_DM9000AEP_H
#define NS_DM9000AEP_H
#include "stm32f10x.h"
#include "user_common.h"
#include "lwip/pbuf.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//DM9000驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/3/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define DM9000_RST		PDout(7)		//DM9000复位引脚
#define DM9000_INT		PGin(6)			//DM9000中断引脚

//DM9000地址结构体
typedef struct
{
	vu16 REG;
	vu16 DATA;
}DM9000_TypeDef;

//使用NOR/SRAM的 Bank1.sector2,地址位HADDR[27,26]=01 A7作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			    
#define DM9000_BASE        ((u32)(0x64000000|0x000000FE))
#define DM9000             ((DM9000_TypeDef *) DM9000_BASE)

#define DM9000_ID			0X90000A46	//DM9000 ID
#define DM9000_PKT_MAX		1536		//DM9000最大接收包长度

#define DM9000_PHY			0X40		//DM9000 PHY寄存器访问标志
//DM9000寄存器
#define DM9000_NCR			0X00
#define DM9000_NSR			0X01
#define DM9000_TCR			0X02
#define DM9000_TSRI			0X03
#define DM9000_TSRII		0X04
#define DM9000_RCR			0X05
#define DM9000_RSR			0X06
#define DM9000_ROCR			0X07	
#define DM9000_BPTR			0X08
#define DM9000_FCTR			0X09
#define DM9000_FCR			0X0A
#define DM9000_EPCR			0X0B
#define DM9000_EPAR			0X0C
#define DM9000_EPDRL		0X0D	
#define DM9000_EPDRH		0X0E
#define DM9000_WCR			0X0F
#define DM9000_PAR			0X10		//物理地址0X10~0X15
#define DM9000_MAR			0X16		//多播地址0X16~0X1D
#define DM9000_GPCR			0X1E
#define DM9000_GPR			0X1F
#define DM9000_TRPAL		0X22
#define DM9000_TRPAH		0X23
#define DM9000_RWPAL		0X24
#define DM9000_RWPAH		0X25

#define DM9000_VIDL			0X28
#define DM9000_VIDH			0X29
#define DM9000_PIDL			0X2A
#define DM9000_PIDH			0X2B

#define DM9000_CHIPR		0X2C
#define DM9000_TCR2			0X2D
#define DM9000_OCR			0X2E
#define DM9000_SMCR			0X2F
#define DM9000_ETXCSR		0X30
#define DM9000_TCSCR		0X31
#define DM9000_RCSCSR		0X32
#define DM9000_MRCMDX		0XF0
#define DM9000_MRCMDX1		0XF1
#define DM9000_MRCMD		0XF2
#define DM9000_MRRL			0XF4
#define DM9000_MRRH			0XF5
#define DM9000_MWCMDX		0XF6
#define DM9000_MWCMD		0XF8
#define DM9000_MWRL			0XFA
#define DM9000_MWRH			0XFB
#define DM9000_TXPLL		0XFC
#define DM9000_TXPLH		0XFD
#define DM9000_ISR			0XFE
#define DM9000_IMR			0XFF

#define NCR_RST             0X01
#define NSR_SPEED           0X80
#define NSR_LINKST         	0X40
#define NSR_WAKEST          0X20
#define NSR_TX2END          0X08
#define NSR_TX1END          0X04
#define NSR_RXOV            0X02

#define RCR_DIS_LONG        0X20
#define RCR_DIS_CRC         0X10
#define RCR_ALL             0X08
#define RCR_RXEN            0X01

#define IMR_PAR             0X80	
#define IMR_ROOI            0X08	
#define IMR_POI             0X04		//使能接收溢出中断
#define IMR_PTI             0X02		//使能发送中断
#define IMR_PRI             0X01		//使能接收中断

#define ISR_LNKCHGS         (1<<5)
#define ISR_ROOS            (1<<3)
#define ISR_ROS             (1<<2)
#define ISR_PTS             (1<<1)
#define ISR_PRS             (1<<0)
#define ISR_CLR_STATUS      (ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS)

//DM9000内部PHY寄存器
#define DM9000_PHY_BMCR		0X00
#define DM9000_PHY_BMSR		0X01
#define DM9000_PHY_PHYID1	0X02
#define DM9000_PHY_PHYID2	0X03
#define DM9000_PHY_ANAR		0X04
#define DM9000_PHY_ANLPAR	0X05
#define DM9000_PHY_ANER		0X06
#define DM9000_PHY_DSCR		0X10
#define DM9000_PHY_DSCSR	0X11
#define DM9000_PHY_10BTCSR	0X12
#define DM9000_PHY_PWDOR	0X13
#define DM9000_PHY_SCR		0X14

//DM9000工作模式定义
enum DM9000_PHY_mode
{
	DM9000_10MHD 	= 	0, 					//10M半双工
	DM9000_100MHD 	= 	1,					//100M半双工	
	DM9000_10MFD 	= 	4, 					//10M全双工
	DM9000_100MFD 	= 	5,					//100M全双工
	DM9000_AUTO  	= 	8, 					//自动协商
};

//DM9000配置结构体
struct dm9000_config
{
	enum DM9000_PHY_mode mode;				//工作模式
	u8  imr_all;							//中断类型 
	u16 queue_packet_len;					//每个数据包大小
	u8  mac_addr[6];						//MAC地址
	u8  multicase_addr[8];					//组播地址
};
extern struct dm9000_config dm9000cfg;		//dm9000配置结构体


u8   DM9000_Init(void);
u16  DM9000_ReadReg(u16 reg);
void DM9000_WriteReg(u16 reg,u16 data);
u16  DM9000_PHY_ReadReg(u16 reg);
void DM9000_PHY_WriteReg(u16 reg,u16 data);
u32  DM9000_Get_DeiviceID(void);
u8   DM9000_Get_SpeedAndDuplex(void);	
void DM9000_Set_PHYMode(u8 mode);
void DM9000_Set_MACAddress(u8 *macaddr);
void DM9000_Set_Multicast(u8 *multicastaddr);
void DM9000_Reset(void);
void DM9000_SendPacket(struct pbuf*p);
struct pbuf *DM9000_Receive_Packet(void);
void DMA9000_ISRHandler(void);
#endif



