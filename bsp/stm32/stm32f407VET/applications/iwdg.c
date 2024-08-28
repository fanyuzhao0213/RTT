/*
 * @Descripttion: �������Ź�Դ�ļ�
 */

#include "iwdg.h"


rt_device_t WdgDev;


static void idle_hook(void)
{
    /* �ڿ����̵߳Ļص�������ι�� */
    rt_device_control(WdgDev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
//    rt_kprintf("feed the dog!\n ");
}

/**
 * @name: WdgInit
 * @msg:  ��ʼ�����Ź�
 * @param [in]
 *         �� 
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int8_t WdgInit(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint32_t timeout = WDT_DEV_TIMEOUT;
	
	//�����豸���Ʋ��ҿ��Ź��豸����ȡ�ļ����
	WdgDev = rt_device_find(WDT_DEV_NAME);
	if(!WdgDev)
	{
		rt_kprintf("find %s failed!\n", WDT_DEV_NAME);
        return RT_ERROR;
	}
	
    //���ÿ��Ź����ʱ��
    ret = rt_device_control(WdgDev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    if (ret != RT_EOK)
    {
        rt_kprintf("set %s timeout failed!\n", WDT_DEV_NAME);
        return RT_ERROR;
    }

    //�������Ź�
    ret = rt_device_control(WdgDev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    if (ret != RT_EOK)
    {
        rt_kprintf("start %s failed!\n", WDT_DEV_NAME);
        return RT_ERROR;
    }

	/* ���ÿ����̻߳ص����� */
    rt_thread_idle_sethook(idle_hook);
	
    return ret;	
	
}
