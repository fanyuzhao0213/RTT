
#ifndef UARTREVDATA_H
#define UARTREVDATA_H

#include <rtthread.h>
#include <rtdevice.h>
#include <rtconfig.h>

#include "uartConfig.h"
#include "Data_RingBuffer.h"
#include "IPCResource.h"


#define UART_NAME       		"uart2"      /* 串口设备名称 */
#define UART_REV_BUF_SIZE 		(2048)

extern void LOG_HEXDUMP(uint8_t *buff, uint16_t len, char* string);
extern struct rt_ringbuffer RecvBLEBuf;
extern void OsTaskmuartRevData(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick);

#endif

