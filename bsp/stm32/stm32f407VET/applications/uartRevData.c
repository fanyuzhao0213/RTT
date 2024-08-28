#include <uartRevData.h>
#include "Protocol_BLE.h"

ALIGN(RT_ALIGN_SIZE);
static char uartRevData_stack[4096];
static struct rt_thread uartRevData_Thread;
// 最大接收数据为1360
CCM_RAM static rt_uint8_t RecvData[1024];

struct rt_ringbuffer RecvBLEBuf;
CCM_RAM static rt_uint8_t sRecvBLERingBuffer[UART_REV_BUF_SIZE];

static void uartRevData(void *param)
{
    rt_uint32_t ValidDataLen = 0;
    rt_uint16_t RecvDataLen = 0;
	
	// BLE与MCU之间的通讯接口配置
    uartCommDevice(UART_NAME);
    rt_ringbuffer_init(&RecvBLEBuf, sRecvBLERingBuffer, sizeof(sRecvBLERingBuffer));
	Data_Buffer_Init();								//databuff初始化
	SemRecvDataFromUartInit();						//信号量初始化
	SendEvent(EVENT_FLAG_RecvDataFromUart);			//发送串口接收线程的事件
	WaitForThreadSync();							//等待事件同步
	rt_kprintf("The currrent Thread is %s!\n", "uartRevData");
	//TimerMSDevInit();
	
	for (;;)
    {
        if (GetSemRecvDataFromUart(100) != RT_EOK)
        {
            ValidDataLen = rt_ringbuffer_data_len(&RecvBLEBuf);
            //清除环形缓冲区中的脏数据--依次循环
            if (ValidDataLen > 1)
            {
                rt_ringbuffer_get(&RecvBLEBuf, RecvData, ValidDataLen);
                memset(RecvData, 0, sizeof(RecvData));
                rt_kprintf("Clean up the dirty data in the ringbuff!\n");
            }
            continue;
        }
		
		//消化积累的信号量(每接收一个字节,接收一个信号量)
        while (RT_EOK == GetSemRecvDataFromUart(0));
		
		for (;;)
        {
            ValidDataLen = rt_ringbuffer_data_len(&RecvBLEBuf);
			rt_kprintf("ValidDataLen : %02x!\n",ValidDataLen);
            // 仅复制协议中的包头和数据长度
            rt_ringbuffer_copy(&RecvBLEBuf, RecvData, MESSAGE_HEAD_BYTE+MESSAGE_LENGTH_BYTE);

            if (ValidDataLen < UART_MESSAGE_MIN_LEN_BYTE)
            {
                //可能没有接收完毕，返回继续接收;如果处理完毕数据，返回继续等待接收。
                break;
            }
            //判断是否是大型机或者小型机的AA/AA BB/BB字头数据
            if( (RecvData[MESSAGE_HEAD1_OFFSET] == UART_HEADER_ID) && (RecvData[MESSAGE_HEAD2_OFFSET] == UART_HEADER_ID))
			{
				rt_kprintf("receive UP AA AA!\n");
			}
            //数据长度不足，可能没有接收完毕，返回继续接收
            RecvDataLen = RecvData[MESSAGE_LENGTH_OFFSET] + (RecvData[MESSAGE_LENGTH_OFFSET+1] << 8);
			rt_kprintf("RecvDataLen : %02x!\n",RecvDataLen);
            if (ValidDataLen < RecvDataLen || RecvDataLen == 0)
            {
                break;
            }

            rt_ringbuffer_get(&RecvBLEBuf, RecvData, RecvDataLen);
            RecvNewMessage(RecvData, RecvDataLen);
            RecvDataLen = 0;
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
void OsTaskmuartRevData(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
    rt_thread_init(&uartRevData_Thread,       // struct rt_thread  *thread
                   "uartRevData",             // const char       *name
                   uartRevData,               // void          (*entry)(void *parameter)
                   RT_NULL,                     // void          *parameter
                   &uartRevData_stack[0],     // void          *stack_start
                   sizeof(uartRevData_stack), // rt_uint32_t    stack_size
                   ThreadPriority,              // rt_uint8_t     priority
                   ThreadTick);                 // rt_uint32_t    tick

    rt_thread_startup(&uartRevData_Thread);
}

