/*
 * @Descripttion: 独立看门狗源文件
 */

#include "iwdg.h"


rt_device_t WdgDev;


static void idle_hook(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(WdgDev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
//    rt_kprintf("feed the dog!\n ");
}

/**
 * @name: WdgInit
 * @msg:  初始化看门狗
 * @param [in]
 *         无 
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int8_t WdgInit(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint32_t timeout = WDT_DEV_TIMEOUT;
	
	//根本设备名称查找看门狗设备，获取文件句柄
	WdgDev = rt_device_find(WDT_DEV_NAME);
	if(!WdgDev)
	{
		rt_kprintf("find %s failed!\n", WDT_DEV_NAME);
        return RT_ERROR;
	}
	
    //设置看门狗溢出时间
    ret = rt_device_control(WdgDev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    if (ret != RT_EOK)
    {
        rt_kprintf("set %s timeout failed!\n", WDT_DEV_NAME);
        return RT_ERROR;
    }

    //启动看门狗
    ret = rt_device_control(WdgDev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    if (ret != RT_EOK)
    {
        rt_kprintf("start %s failed!\n", WDT_DEV_NAME);
        return RT_ERROR;
    }

	/* 设置空闲线程回调函数 */
    rt_thread_idle_sethook(idle_hook);
	
    return ret;	
	
}
