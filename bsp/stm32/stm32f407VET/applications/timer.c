/*
 * @Descripttion: 硬件定时器源文件
 */

#include "timer.h"

#define HWTIMER_DEV_NAME_ms "timer3"
#define HWTIMER_DEV_NAME_s 	"timer4"
rt_device_t TimerDev = RT_NULL;

/**
 * @name: TimerMSDevInit
 * @msg:  定时器初始化(ms)
 * @param [in]
 *         *Callback: 定时器回调函数
 * @param [out]
 *         无 
 * @return 1:初始化失败；0：初始化成功
 * @note:  1）默认计数频率为1M,则最小的计数单位为us;
 *         2）默认溢出时间为1ms.
 */
rt_uint8_t TimerMSDevInit(rt_err_t (*Callback)(rt_device_t dev, rt_size_t size))
{
    rt_err_t ret = RT_EOK;
    rt_hwtimer_mode_t TimerMode = HWTIMER_MODE_PERIOD;
    //定时器超时时间
    rt_hwtimerval_t TimerTimeOut = 
    {
        .sec = 0,
        .usec = 1000			//1000us为1ms
    };

    TimerDev = rt_device_find(HWTIMER_DEV_NAME_ms);
    if (RT_NULL == TimerDev)
    {
        rt_kprintf("Can't find %s device!\n", HWTIMER_DEV_NAME_ms);
        return RT_ERROR;
    }

    ret = rt_device_open(TimerDev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", HWTIMER_DEV_NAME_ms);
        return ret;
    }

    
    //设置定时模式
    ret = rt_device_control(TimerDev, HWTIMER_CTRL_MODE_SET, &TimerMode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode of %s device failed!\n", HWTIMER_DEV_NAME_ms);
        return ret;
    }
    //设置超时回调函数
    rt_device_set_rx_indicate(TimerDev, Callback);

    //设置超时时间并启动定时器
    if(rt_device_write(TimerDev, 0, &TimerTimeOut, sizeof(TimerTimeOut)) != sizeof(TimerTimeOut))
    {
        rt_kprintf("Set timeout value failed!\n");
        return RT_ERROR;
    }

    return ret;
}

/**
 * @name: TimerSDevInit
 * @msg:  定时器初始化  S  
 * @param [in]
 *         *Callback: 定时器回调函数
 * @param [out]
 *         无 
 * @return 1:初始化失败；0：初始化成功
 * @note:  1）默认计数频率为1M,则最小的计数单位为us;
 *         2）默认溢出时间为1ms.
 */
rt_uint8_t TimerSDevInit(rt_err_t (*Callback)(rt_device_t dev, rt_size_t size))
{
    rt_err_t ret = RT_EOK;
    rt_hwtimer_mode_t TimerMode = HWTIMER_MODE_PERIOD;
    //定时器超时时间
    rt_hwtimerval_t TimerTimeOut = 
    {
        .sec = 1,
        .usec = 0			//1000us为1ms
    };

    TimerDev = rt_device_find(HWTIMER_DEV_NAME_s);
    if (RT_NULL == TimerDev)
    {
        rt_kprintf("Can't find %s device!\n", HWTIMER_DEV_NAME_s);
        return RT_ERROR;
    }

    ret = rt_device_open(TimerDev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", HWTIMER_DEV_NAME_s);
        return ret;
    }

    
    //设置定时模式
    ret = rt_device_control(TimerDev, HWTIMER_CTRL_MODE_SET, &TimerMode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode of %s device failed!\n", HWTIMER_DEV_NAME_s);
        return ret;
    }
    //设置超时回调函数
    rt_device_set_rx_indicate(TimerDev, Callback);

    //设置超时时间并启动定时器
    if(rt_device_write(TimerDev, 0, &TimerTimeOut, sizeof(TimerTimeOut)) != sizeof(TimerTimeOut))
    {
        rt_kprintf("Set timeout value failed!\n");
        return RT_ERROR;
    }

    return ret;
}

