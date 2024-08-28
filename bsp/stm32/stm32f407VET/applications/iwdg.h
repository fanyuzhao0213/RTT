/*
 * @Descripttion: 独立看门狗头文件
 */

#ifndef IWDG_H
#define IWDG_H

#include <rtthread.h>
#include <rtdevice.h>

#define WDT_DEV_NAME "wdt"
//溢出时间单位为秒
#define WDT_DEV_TIMEOUT 2

extern rt_device_t WdgDev;

rt_int8_t WdgInit(void);

#endif

