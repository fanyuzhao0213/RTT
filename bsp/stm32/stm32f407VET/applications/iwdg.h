/*
 * @Descripttion: �������Ź�ͷ�ļ�
 */

#ifndef IWDG_H
#define IWDG_H

#include <rtthread.h>
#include <rtdevice.h>

#define WDT_DEV_NAME "wdt"
//���ʱ�䵥λΪ��
#define WDT_DEV_TIMEOUT 2

extern rt_device_t WdgDev;

rt_int8_t WdgInit(void);

#endif

