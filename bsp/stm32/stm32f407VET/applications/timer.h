/*
 * @Descripttion: 硬件定时器头文件
 */

#ifndef TIMER_H
#define TIMER_H

#include <rtdevice.h>

extern rt_uint8_t TimerMSDevInit(rt_err_t (*Callback)(rt_device_t dev, rt_size_t size));
extern rt_uint8_t TimerSDevInit(rt_err_t (*Callback)(rt_device_t dev, rt_size_t size));
#endif

