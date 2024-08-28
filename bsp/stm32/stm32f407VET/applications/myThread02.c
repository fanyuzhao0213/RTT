#include <myThread02.h>

ALIGN(RT_ALIGN_SIZE);
static char myThread02_stack[4096];
static struct rt_thread myThread02_Thread;

static void myThread02(void *param)
{
    rt_uint32_t count = 0;

    /* 线程 2 拥有较高的优先级，以抢占线程 1 而获得执行 */
    for (count = 0; count < 10 ; count++)
    {
        /* 线程 2 打印计数值 */
//        rt_kprintf("thread2 count: %d\n", count);
    }
    rt_kprintf("thread2 exit\n");
    /* 线程 2 运行结束后也将自动被系统脱离 */
}

/**
 * @name: OsTaskSendDataToBLEInit
 * @msg: 亲机向蓝牙发送数据线程初始化
 * @param [in]
 *        ThreadPriority 线程优先级
 *        ThreadTick     线程时间片
 * @param [out]
 *         无
 * @return 无
 * @note: 无
 */
void OsTaskmyThread02(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
    rt_thread_init(&myThread02_Thread,       // struct rt_thread  *thread
                   "SendDataToBLE",             // const char       *name
                   myThread02,               // void          (*entry)(void *parameter)
                   RT_NULL,                     // void          *parameter
                   &myThread02_stack[0],     // void          *stack_start
                   sizeof(myThread02_stack), // rt_uint32_t    stack_size
                   ThreadPriority,              // rt_uint8_t     priority
                   ThreadTick);                 // rt_uint32_t    tick

    rt_thread_startup(&myThread02_Thread);
}

