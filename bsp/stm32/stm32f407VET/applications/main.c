/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define OS_TASK_STRUCT OsTaskStruct *
//����̸߳���Ϊ10���ɸ���ʵ����Ŀ�޸�
#define OS_TASK_FUNC_ID_END 0x0A
//����һ������ָ��
typedef void (*pOsTaskFuncInit)(rt_uint8_t,rt_uint32_t);

//�����߳̽ṹ��
typedef struct 
{
	const rt_uint8_t FuncId;			//�߳����
	const pOsTaskFuncInit TaskInit;		//����
	const rt_uint8_t priority;			//�߳����ȼ�
	const rt_uint32_t Tick;				//�߳�ռ�õ�Tick
}OsTaskStruct;

//����һ���̵߳ĳ�ʼ����
static const OsTaskStruct OsTaskInitTable[] = 
{
    //Task ID    TaskInitPointer                      Thread Priority      Thread Tick  
    {0,          			OsTaskmyThread01,            	1,                     10},
    {1,          			OsTaskmyThread02,          		3,                     	5},
	{2,          			OsTaskmuartRevData,          	3,                     	5},	
	{3,						OsTaskuartParseData,			4,						5},
	{4,						OsMyOledThreadInit,			3,						5},
    {OS_TASK_FUNC_ID_END,          	RT_NULL,            	1,              5},
};

/**
 * @name: OsTaskInit
 * @msg: ���̳߳�ʼ��
 * @param [in]
 *         ��
 * @param [out]
 *        ��
 * @return ��
 * @note:
 */
static void OsTaskInit(void)
{
    OS_TASK_STRUCT pOsTask = (OsTaskStruct *)&OsTaskInitTable[0];

    while (pOsTask->FuncId < OS_TASK_FUNC_ID_END)
    {
        if (pOsTask->TaskInit != RT_NULL)
        {
            pOsTask->TaskInit(pOsTask->priority, pOsTask->Tick);
        }
        pOsTask++;
    }
}


/* defined the LED0 pin: PB2 */
#define LED0_PIN    GET_PIN(B, 2)

rt_err_t timerSCallback(rt_device_t dev, rt_size_t size)
{
	static uint8_t status = 0;
	if(status == 0)
	{
		status = 1;
		rt_pin_write(LED0_PIN, PIN_HIGH);
	}
	else
	{
		status = 0;
		rt_pin_write(LED0_PIN, PIN_LOW);
	}
	return 0;
}


int main(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	EventCtrlInit();					//�¼�����ʼ��
//	WdgInit();							//���Ź���ʼ��
	TimerSDevInit(timerSCallback);		//��ʱ����ʼ��
	OsTaskInit();						//���̳߳�ʼ��
	SendEvent(EVENT_FLAG_Main);			//����main�̵߳��¼�
	WaitForThreadSync();				//�ȴ��¼�ͬ��
	rt_kprintf("The currrent Thread is %s!\n", "main");
    while (1)
    {
        rt_thread_mdelay(500);			//��500ms��ʱΪ���ÿ����߳����У������߳���ι��
    }

    return RT_EOK;
}
