#include "display_iic.h"
#include "oled.h"

ALIGN(RT_ALIGN_SIZE);
#define OLED_I2C_BUS_NAME          "i2c1"
static rt_uint32_t MyDisplayIICThreadInit_stack = 1024;
struct rt_i2c_bus_device *oled_i2c_bus;

rt_uint8_t test;

void updateWord(void)
{
	OLED_SetIntegerNum(22, 34, test, 3, 1, FONT_12X12);
	test++;
}

//void updatePrintf(void)
//{
//	OLED_Printf("test = %d\n", test++);
//}

void updateDisplay(void)
{
	OLED_ShowTask();
}

typedef enum
{
    TASK_STOP = 0,
    TASK_RUN,
}TimerTaskRunStatus;

typedef struct
{
    rt_uint8_t mRunFlag;
    rt_uint32_t mTimerCountdown;
    rt_uint32_t mReloadIntervalTimer;
    void (*mpFunc)();
}TimerTaskInfo;

#define TIMER_TASK_MAX_COUNTER (3)

static TimerTaskInfo TimerTask[TIMER_TASK_MAX_COUNTER] =
{
    {TASK_RUN,   100,       		100,   				updateDisplay},
//	{TASK_RUN,   100,  				100,				updatePrintf},
//	{TASK_RUN,   1000,   			1000, 				updateWord}
};

void TimerTaskRemarks(void)
{
    for (int i = 0; i < TIMER_TASK_MAX_COUNTER; i++)
    {
        if (TimerTask[i].mTimerCountdown)
        {
            TimerTask[i].mTimerCountdown--;
            if (0 == TimerTask[i].mTimerCountdown)
            {
                TimerTask[i].mRunFlag = TASK_RUN;
            }
        }
    }
}

void TimerTaskProcess(void)
{
    for (int i = 0; i < TIMER_TASK_MAX_COUNTER; i++)
    {
        if (TASK_RUN == TimerTask[i].mRunFlag)
        {
            TimerTask[i].mpFunc();
            TimerTask[i].mRunFlag = TASK_STOP;
            TimerTask[i].mTimerCountdown = TimerTask[i].mReloadIntervalTimer;
        }
    }
}


rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];
    struct rt_i2c_msg msgs;
    buf[0] = reg; //cmd
    buf[1] = data;
    msgs.addr = OLED_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 2;

    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

//mså®šæ—¶å™?
rt_err_t mstimerSCallback(rt_device_t dev, rt_size_t size)
{
	TimerTaskRemarks();
	return 0;
}

void MyDisplayIICThread(void *parameter)
{
	rt_uint8_t pin1 = GET_PIN(C, 6);
	rt_uint8_t pin2 = GET_PIN(C, 7);
	rt_kprintf("pin1:%d,pin2:%d\n", pin1,pin2);

	rt_uint8_t pin3 = GET_PIN(D, 4);
	rt_uint8_t pin4 = GET_PIN(D, 5);
	rt_kprintf("pin3:%d,pin4:%d\n", pin3,pin4);
	
    oled_i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(OLED_I2C_BUS_NAME);
    if (oled_i2c_bus == RT_NULL)
    {
        rt_kprintf("iic sample run failed! can't find %s device!\n", OLED_I2C_BUS_NAME);
    }else{
		rt_kprintf("iic oled run successs! find %s device!\n", OLED_I2C_BUS_NAME);
	}

	rt_kprintf("The currrent Thread is %s!\n", "MyDisplayIICThread");

	TimerMSDevInit(mstimerSCallback);
	OLED_Init();
	OLED_Clear(OLED_BLACK);
//	OLED_Clear(OLED_BLACK);
//	OLED_SetFill(0, 0, 128, 64, OLED_BLACK);
//    OLED_DrawGraphic(20, 0, "bootLog", 1);
//    OLED_SyncScreen(20, 0, 128, 64);
//    rt_thread_mdelay(1500);
//    OLED_SetFill(0, 0, 128, 64, OLED_BLACK);
    OLED_SetColor(OLED_WHITE, OLED_BLACK);
    OLED_SetText(40, 2, "Î÷°²Í¾¶¡", 0, FONT_12X12);
    OLED_SetColor(OLED_BLACK, OLED_WHITE);
    OLED_DrawCircle(68, 40, 20, 4);
	while(1)
	{
		TimerTaskProcess();
        rt_thread_mdelay(50);
	}
}

void OsDisplayIICThreadInit(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
    rt_thread_t tid;

    tid = rt_thread_create("displayIIC_thread",
                  MyDisplayIICThread,
                  RT_NULL,
                  MyDisplayIICThreadInit_stack,
                  ThreadPriority,
                  ThreadTick);

    if (tid != RT_NULL)
    {
		rt_kprintf("thread  OsDisplayIICThreadInit  start success!!\n");
    	rt_thread_startup(tid);
	}

}

