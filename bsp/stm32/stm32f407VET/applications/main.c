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
//最大线程个数为10，可根据实际项目修改
#define OS_TASK_FUNC_ID_END 0x0A
//定义一个函数指针
typedef void (*pOsTaskFuncInit)(rt_uint8_t,rt_uint32_t);

//定义线程结构体
typedef struct 
{
	const rt_uint8_t FuncId;			//线程序号
	const pOsTaskFuncInit TaskInit;		//函数
	const rt_uint8_t priority;			//线程优先级
	const rt_uint32_t Tick;				//线程占用的Tick
}OsTaskStruct;

//定义一个线程的初始化表
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
 * @msg: 各线程初始化
 * @param [in]
 *         无
 * @param [out]
 *        无
 * @return 无
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
	EventCtrlInit();					//事件集初始化
//	WdgInit();							//看门狗初始化
	TimerSDevInit(timerSCallback);		//定时器初始化
	OsTaskInit();						//各线程初始化
	SendEvent(EVENT_FLAG_Main);			//发送main线程的事件
	WaitForThreadSync();				//等待事件同步
	rt_kprintf("The currrent Thread is %s!\n", "main");
    while (1)
    {
        rt_thread_mdelay(500);			//加500ms延时为了让空闲线程运行，空闲线程在喂狗
    }

    return RT_EOK;
}
