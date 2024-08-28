/*
 * @Descripttion: 事件集初始化头文件
 */

#ifndef EVENT_INIT_H
#define EVENT_INIT_H

#include <rtthread.h>

//线程同步事件集相关
#define EVENT_FLAG_RecvDataFromUart                    	(1 << 0)
#define EVENT_FLAG_ParseDataFromUart                  	(1 << 1)
#define EVENT_FLAG_Main                                	(1 << 2)

extern rt_err_t EventCtrlInit(void);
extern rt_err_t SendEvent(rt_uint32_t EventFlag);
extern rt_err_t WaitForThreadSync(void);

#endif
