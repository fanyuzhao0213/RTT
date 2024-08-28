/*
 * @Descripttion: Ӳ����ʱ��Դ�ļ�
 */

#include "timer.h"

#define HWTIMER_DEV_NAME_ms "timer3"
#define HWTIMER_DEV_NAME_s 	"timer4"
rt_device_t TimerDev = RT_NULL;

/**
 * @name: TimerMSDevInit
 * @msg:  ��ʱ����ʼ��(ms)
 * @param [in]
 *         *Callback: ��ʱ���ص�����
 * @param [out]
 *         �� 
 * @return 1:��ʼ��ʧ�ܣ�0����ʼ���ɹ�
 * @note:  1��Ĭ�ϼ���Ƶ��Ϊ1M,����С�ļ�����λΪus;
 *         2��Ĭ�����ʱ��Ϊ1ms.
 */
rt_uint8_t TimerMSDevInit(rt_err_t (*Callback)(rt_device_t dev, rt_size_t size))
{
    rt_err_t ret = RT_EOK;
    rt_hwtimer_mode_t TimerMode = HWTIMER_MODE_PERIOD;
    //��ʱ����ʱʱ��
    rt_hwtimerval_t TimerTimeOut = 
    {
        .sec = 0,
        .usec = 1000			//1000usΪ1ms
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

    
    //���ö�ʱģʽ
    ret = rt_device_control(TimerDev, HWTIMER_CTRL_MODE_SET, &TimerMode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode of %s device failed!\n", HWTIMER_DEV_NAME_ms);
        return ret;
    }
    //���ó�ʱ�ص�����
    rt_device_set_rx_indicate(TimerDev, Callback);

    //���ó�ʱʱ�䲢������ʱ��
    if(rt_device_write(TimerDev, 0, &TimerTimeOut, sizeof(TimerTimeOut)) != sizeof(TimerTimeOut))
    {
        rt_kprintf("Set timeout value failed!\n");
        return RT_ERROR;
    }

    return ret;
}

/**
 * @name: TimerSDevInit
 * @msg:  ��ʱ����ʼ��  S  
 * @param [in]
 *         *Callback: ��ʱ���ص�����
 * @param [out]
 *         �� 
 * @return 1:��ʼ��ʧ�ܣ�0����ʼ���ɹ�
 * @note:  1��Ĭ�ϼ���Ƶ��Ϊ1M,����С�ļ�����λΪus;
 *         2��Ĭ�����ʱ��Ϊ1ms.
 */
rt_uint8_t TimerSDevInit(rt_err_t (*Callback)(rt_device_t dev, rt_size_t size))
{
    rt_err_t ret = RT_EOK;
    rt_hwtimer_mode_t TimerMode = HWTIMER_MODE_PERIOD;
    //��ʱ����ʱʱ��
    rt_hwtimerval_t TimerTimeOut = 
    {
        .sec = 1,
        .usec = 0			//1000usΪ1ms
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

    
    //���ö�ʱģʽ
    ret = rt_device_control(TimerDev, HWTIMER_CTRL_MODE_SET, &TimerMode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode of %s device failed!\n", HWTIMER_DEV_NAME_s);
        return ret;
    }
    //���ó�ʱ�ص�����
    rt_device_set_rx_indicate(TimerDev, Callback);

    //���ó�ʱʱ�䲢������ʱ��
    if(rt_device_write(TimerDev, 0, &TimerTimeOut, sizeof(TimerTimeOut)) != sizeof(TimerTimeOut))
    {
        rt_kprintf("Set timeout value failed!\n");
        return RT_ERROR;
    }

    return ret;
}

