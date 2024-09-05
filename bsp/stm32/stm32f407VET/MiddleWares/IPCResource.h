/*
 * @Descripttion: 系统中涉及的IPC资源头文件
 */

#ifndef IPC_RESOURCE_H
#define IPC_RESOURCE_H

#include <rtthread.h>
#include "EventInit.h"

//uart数据接收信号量*
extern rt_int8_t SemRecvDataFromUartInit(void);
void ReleaseSemRecvDataFromUart(void);
rt_uint8_t GetSemRecvDataFromUart(rt_int32_t TimeOut);

//uplinkbuffer互斥锁
rt_int8_t MutexUartRingBufInit(void);
void ReleaseMutexUartRingBuf(void);
rt_uint8_t GetMutexUartRingBuf(rt_int32_t TimeOut);


//uplinkbuffer数据解析信号量
rt_int8_t SemParseUplinkDataInit(void);
void ReleaseSemParseUplinkData(void);
rt_uint8_t GetSemParseUplinkData(rt_int32_t TimeOut);


rt_int8_t SemSemNetInfoInit(void);
void ReleaseSemNetInfo(void);
rt_uint8_t GetSemNetInfo(rt_int32_t TimeOut);

#endif
