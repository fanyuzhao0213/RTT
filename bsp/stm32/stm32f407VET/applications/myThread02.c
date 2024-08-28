#include <myThread02.h>

ALIGN(RT_ALIGN_SIZE);
static char myThread02_stack[4096];
static struct rt_thread myThread02_Thread;

static void myThread02(void *param)
{
    rt_uint32_t count = 0;

    /* �߳� 2 ӵ�нϸߵ����ȼ�������ռ�߳� 1 �����ִ�� */
    for (count = 0; count < 10 ; count++)
    {
        /* �߳� 2 ��ӡ����ֵ */
//        rt_kprintf("thread2 count: %d\n", count);
    }
    rt_kprintf("thread2 exit\n");
    /* �߳� 2 ���н�����Ҳ���Զ���ϵͳ���� */
}

/**
 * @name: OsTaskSendDataToBLEInit
 * @msg: �׻����������������̳߳�ʼ��
 * @param [in]
 *        ThreadPriority �߳����ȼ�
 *        ThreadTick     �߳�ʱ��Ƭ
 * @param [out]
 *         ��
 * @return ��
 * @note: ��
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

