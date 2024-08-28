#include <uartRevData.h>
#include "Protocol_BLE.h"

ALIGN(RT_ALIGN_SIZE);
static char uartRevData_stack[4096];
static struct rt_thread uartRevData_Thread;
// ����������Ϊ1360
CCM_RAM static rt_uint8_t RecvData[1024];

struct rt_ringbuffer RecvBLEBuf;
CCM_RAM static rt_uint8_t sRecvBLERingBuffer[UART_REV_BUF_SIZE];

static void uartRevData(void *param)
{
    rt_uint32_t ValidDataLen = 0;
    rt_uint16_t RecvDataLen = 0;
	
	// BLE��MCU֮���ͨѶ�ӿ�����
    uartCommDevice(UART_NAME);
    rt_ringbuffer_init(&RecvBLEBuf, sRecvBLERingBuffer, sizeof(sRecvBLERingBuffer));
	Data_Buffer_Init();								//databuff��ʼ��
	SemRecvDataFromUartInit();						//�ź�����ʼ��
	SendEvent(EVENT_FLAG_RecvDataFromUart);			//���ʹ��ڽ����̵߳��¼�
	WaitForThreadSync();							//�ȴ��¼�ͬ��
	rt_kprintf("The currrent Thread is %s!\n", "uartRevData");
	//TimerMSDevInit();
	
	for (;;)
    {
        if (GetSemRecvDataFromUart(100) != RT_EOK)
        {
            ValidDataLen = rt_ringbuffer_data_len(&RecvBLEBuf);
            //������λ������е�������--����ѭ��
            if (ValidDataLen > 1)
            {
                rt_ringbuffer_get(&RecvBLEBuf, RecvData, ValidDataLen);
                memset(RecvData, 0, sizeof(RecvData));
                rt_kprintf("Clean up the dirty data in the ringbuff!\n");
            }
            continue;
        }
		
		//�������۵��ź���(ÿ����һ���ֽ�,����һ���ź���)
        while (RT_EOK == GetSemRecvDataFromUart(0));
		
		for (;;)
        {
            ValidDataLen = rt_ringbuffer_data_len(&RecvBLEBuf);
			rt_kprintf("ValidDataLen : %02x!\n",ValidDataLen);
            // ������Э���еİ�ͷ�����ݳ���
            rt_ringbuffer_copy(&RecvBLEBuf, RecvData, MESSAGE_HEAD_BYTE+MESSAGE_LENGTH_BYTE);

            if (ValidDataLen < UART_MESSAGE_MIN_LEN_BYTE)
            {
                //����û�н�����ϣ����ؼ�������;�������������ݣ����ؼ����ȴ����ա�
                break;
            }
            //�ж��Ƿ��Ǵ��ͻ�����С�ͻ���AA/AA BB/BB��ͷ����
            if( (RecvData[MESSAGE_HEAD1_OFFSET] == UART_HEADER_ID) && (RecvData[MESSAGE_HEAD2_OFFSET] == UART_HEADER_ID))
			{
				rt_kprintf("receive UP AA AA!\n");
			}
            //���ݳ��Ȳ��㣬����û�н�����ϣ����ؼ�������
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
 * @msg: �׻����������������̳߳�ʼ��
 * @param [in]
 *        ThreadPriority �߳����ȼ�
 *        ThreadTick     �߳�ʱ��Ƭ
 * @param [out]
 *         ��
 * @return ��
 * @note: ��
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

