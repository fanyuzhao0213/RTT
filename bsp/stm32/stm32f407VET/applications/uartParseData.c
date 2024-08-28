#include <uartParseData.h>
#include "IPCResource.h"
#include "getDataFromRingBuf.h"
#include "Data_RingBuffer.h"

ALIGN(RT_ALIGN_SIZE);
static char uartParseData_stack[4096];
static struct rt_thread uartParseData_Thread;
CCM_RAM rt_uint8_t Current_Message_Buffer[1400];
static rt_uint16_t MessageLen;
static rt_uint8_t  ServerResponse[512];

void LOG_HEXDUMP(uint8_t *buff, uint16_t len, char* string)
{
	rt_kprintf("%s\n", string);
	for(uint16_t i =0 ; i< len; i++)
	{
		rt_kprintf("%02x",buff[i]);
	}
	rt_kprintf("\n");
}

static void uartParseData(void *param)
{
	int RetValue;
	rt_uint16_t MassageID = UART_MESSAGE_ID_INVALID;
	MutexUartRingBufInit();								//uart数据互斥锁初始化
	SemParseUplinkDataInit();							//解析uart数据信号量初始化
	SendEvent(EVENT_FLAG_ParseDataFromUart);			//发送串口解析线程的事件
	WaitForThreadSync();								//等待事件同步

	rt_kprintf("The currrent Thread is %s!\n", "uartParseData");
	for (;;)
    {
        //信号量在被使用时进入if判断且会等待100ms，空闲时进入else判断
        if (GetSemParseUplinkData(100) != RT_EOK)
        {
			//此时没有串口的数据
			//可以做一些空闲操作
        }
        else   //从UplinkDataRingBuffer中提取数据
        {
            if (GetMessagesFromUplinkRingBuf(Current_Message_Buffer, &MessageLen))
            {
				LOG_HEXDUMP(Current_Message_Buffer,MessageLen,"get data from ringbuff");
				MassageID = Current_Message_Buffer[RINGBUFFER_MESSAGE_FUNCID_OFFSET] + \
							(Current_Message_Buffer[RINGBUFFER_MESSAGE_FUNCID_OFFSET+1]<<8);
				
				rt_memset(ServerResponse, 0x00, sizeof(ServerResponse));
				RetValue = Server_Decode_Message(Current_Message_Buffer, ServerResponse);
            }
        }
    }
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
void OsTaskuartParseData(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
    rt_thread_init(&uartParseData_Thread,       // struct rt_thread  *thread
                   "uartParseData",             // const char       *name
                   uartParseData,               // void          (*entry)(void *parameter)
                   RT_NULL,                     // void          *parameter
                   &uartParseData_stack[0],     // void          *stack_start
                   sizeof(uartParseData_stack), // rt_uint32_t    stack_size
                   ThreadPriority,              // rt_uint8_t     priority
                   ThreadTick);                 // rt_uint32_t    tick

    rt_thread_startup(&uartParseData_Thread);
}

