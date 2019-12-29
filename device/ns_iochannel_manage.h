#ifndef NS_IOCHANNEL_MANAGE_H
#define NS_IOCHANNEL_MANAGE_H
/*
某些设备可能需要IO通道的操作和支持，IO通道管理和IO通道中断在这里处理
比如stm32可以借助事件机制，实现IO通道功能
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "ns_config.h"
#include "ns.h"

#ifdef __cplusplus
}
#endif
#endif