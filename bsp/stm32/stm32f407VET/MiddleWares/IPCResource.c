/*
 * @Descripttion: 系统中涉及的IPC资源源文件
 */

#include "IPCResource.h"

/******************************蓝牙数据接收信号量*******************************/
static struct rt_semaphore SemRecvDataFromUart;
/**
 * @name: SemRecvDataFromUartInit
 * @msg:  蓝牙数据接收信号量初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t SemRecvDataFromUartInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_init(&SemRecvDataFromUart, "SemRecvDataFromUart", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s semaphore failed!\n", "SemRecvDataFromUart");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: SemRecvDataFromUar
 * @msg:  释放蓝牙数据接收信号量
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
void ReleaseSemRecvDataFromUart(void)
{
    rt_sem_release(&SemRecvDataFromUart);
}
/**
 * @name: GetSemRecvDataFromUart
 * @msg:  获得蓝牙数据接收信号量
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t GetSemRecvDataFromUart(rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_take(&SemRecvDataFromUart, TimeOut);
    if (result != RT_EOK)
    {
        return RT_ERROR;
    }
    
    return result;
}

/******************************蓝牙数据解析信号量*******************************/
static struct rt_semaphore  SemParseUplinkData;

/******************************服务器解析信号量*******************************/
static struct rt_semaphore  SemParseServerData;


/*服务器相关信号量接口*/
rt_int8_t SemParseServerDataInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_init(&SemParseServerData, "SemParseServerData", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s semaphore failed!\n", "SemParseServerData");
        return RT_ERROR;
    }
    
    return result;
}

void ReleaseSemParseServerData(void)
{
    rt_sem_release(&SemParseServerData);
}


rt_uint8_t GetSemParseServerData(rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_take(&SemParseServerData, TimeOut);
    if (result != RT_EOK)
    {
        // rt_kprintf("Fail to get %s !", "SemParseUplinkData");
        return RT_ERROR;
    }
    
    return result;
}


/**
 * @name: SemParseUplinkDataInit
 * @msg:  蓝牙数据解析信号量初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t SemParseUplinkDataInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_init(&SemParseUplinkData, "SemParseUplinkData", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s semaphore failed!\n", "SemParseUplinkData");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: ReleaseSemParseUplinkData
 * @msg:  释放uart数据解析信号量
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
void ReleaseSemParseUplinkData(void)
{
    rt_sem_release(&SemParseUplinkData);
}

/**
 * @name: GetSemParseUplinkData
 * @msg:  获得蓝牙数据解析信号量
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t GetSemParseUplinkData(rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_take(&SemParseUplinkData, TimeOut);
    if (result != RT_EOK)
    {
        // rt_kprintf("Fail to get %s !", "SemParseUplinkData");
        return RT_ERROR;
    }
    
    return result;
}

/******************************下行数据处理信号量*******************************/
static struct rt_semaphore  SemSendDataToUart;

/**
 * @name: SemSendDataToUartInit
 * @msg:  串口处理下行数据信号量初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t SemSendDataToUartInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_init(&SemSendDataToUart, "SemSendDataToUart", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        // rt_kprintf("Init %s semaphore failed!", "SemSendDataToUart");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: ReleaseSemSendDataToUart
 * @msg:  释放串口处理下行数据信号量
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void ReleaseSemSendDataToUart(void)
{
    rt_sem_release(&SemSendDataToUart);
}

/**
 * @name: GetSemSendDataToUart
 * @msg:  获得下行数据处理信号量
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t GetSemSendDataToUart(rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_take(&SemSendDataToUart, TimeOut);
    if (result != RT_EOK)
    {
        // rt_kprintf("Fail to get %s !", "SemSendDataToUart");
        return RT_ERROR;
    }
    
    return result;
}

/******************************串口发送同级应答消息队列*******************************/
static struct rt_messagequeue MqUartTxAck;
#define MQ_UART_TX_ACK_MSG_SIZE   (20)
#define MQ_UART_TX_ACK_MAX_MSGS   (5)
//消息内存池大小为（消息头大小（4字节，结构体指针类型）+ 消息大小）* 消息队列大小
static rt_uint8_t MqUartTxAckMsgPool[(MQ_UART_TX_ACK_MSG_SIZE + 4) * MQ_UART_TX_ACK_MAX_MSGS];

/**
 * @name: MqUartTxAckInit
 * @msg:  串口接收同级应答消息队列初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t MqUartTxAckInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_mq_init(&MqUartTxAck, "MqUartTxAck", MqUartTxAckMsgPool, MQ_UART_TX_ACK_MSG_SIZE, sizeof(MqUartTxAckMsgPool), RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s MessageQueue failed!", "MqUartTxAck");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: SendMqUartTxAck
 * @msg:  发送串口接收同级应答消息队列
 * @param [in]
 *         *SendBuffer: 待发送的数据buffer
 *         DataSize: 需要发送的数据个数
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t SendMqUartTxAck(rt_uint8_t *SendBuffer, rt_uint8_t DataSize)
{
    rt_err_t result = RT_EOK;

    result = rt_mq_send(&MqUartTxAck, SendBuffer, DataSize);
    if (result == -RT_EFULL)
    {
        rt_kprintf("The MessageQueue of %s is full!", "MqUartTxAck");
        return RT_ERROR;
    }
    else if(result == -RT_ERROR)
    {
        rt_kprintf("The Length of Msg that will be sent is greater than the max length of a msg in %s MessageQueue!\n", "MqUartTxAck");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: RecvMqUartTxAck
 * @msg:  发送串口接收同级应答消息队列
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         *RecvBuffer: 待存放数据buffer
 *         DataSize: 需要发送的数据个数
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t RecvMqUartTxAck(rt_uint8_t *RecvBuffer, rt_uint8_t DataSize, rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_mq_recv(&MqUartTxAck, RecvBuffer, DataSize, TimeOut);
    if (result != RT_EOK)
    {
        // rt_kprintf("Fail to Recv data from %s MessageQueue!", "MqUartTxAck");
        return RT_ERROR;
    }
    
    return result;
}

/******************************下行数据缓存DownlinkDataRingBuffer互斥量*******************************/
static struct rt_mutex MutexDownlinkRingBuf;

/**
 * @name: MutexDownlinkRingBufInit
 * @msg:  下行数据缓存DownlinkDataRingBuffer互斥量初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t MutexDownlinkRingBufInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_mutex_init(&MutexDownlinkRingBuf, "MutexDownlinkRingBuf", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s Mutex failed!", "MutexDownlinkRingBuf");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: ReleaseMutexDownlinkRingBuf
 * @msg:  释放下行数据缓存互斥量
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void ReleaseMutexDownlinkRingBuf(void)
{
    rt_mutex_release(&MutexDownlinkRingBuf);
}

/**
 * @name: GetMutexDownlinkRingBuf
 * @msg:  获得操作下行数据缓存DownlinkDataRingBuffer的互斥量
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t GetMutexDownlinkRingBuf(rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_mutex_take(&MutexDownlinkRingBuf, TimeOut);
    if (result != RT_EOK)
    {
        // rt_kprintf("Fail to get %s !", "MutexDownlinkRingBuf");
        return RT_ERROR;
    }
    
    return result;
}

/******************************上行数据缓存UpinkDataRingBuffer互斥量*******************************/
static struct rt_mutex MutexUartRingBuf;

/**
 * @name: MutexUartRingBuf
 * @msg:  数据缓存MutexUartRingBuf互斥量初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t MutexUartRingBufInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_mutex_init(&MutexUartRingBuf, "MutexUartRingBuf", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s Mutex failed!", "MutexUartRingBuf");
        return RT_ERROR;
    }
	rt_kprintf("MutexUartRingBufInitx success!!\n");
    
    return result;
}

/**
 * @name: ReleaseMutexUartRingBuf
 * @msg:  释放上行数据缓存互斥量
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void ReleaseMutexUartRingBuf(void)
{
    rt_mutex_release(&MutexUartRingBuf);
}

/**
 * @name: GetMutexUartRingBuf
 * @msg:  获得操作上行数据缓存MutexUartRingBuf的互斥量
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t GetMutexUartRingBuf(rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;
    result = rt_mutex_take(&MutexUartRingBuf, TimeOut);
    if (result != RT_EOK)
    {
        // rt_kprintf("Fail to get %s !", "MutexUplinkRingBuf");
        return RT_ERROR;
    }
    
    return result;
}

/******************************系统异常邮箱*******************************/
static struct rt_mailbox MailboxSystemExcept;
#define MAX_MAIL_COUNT (8)
static rt_uint8_t MailboxPool[4*MAX_MAIL_COUNT];
/**
 * @name: MailboxSystemExceptInit
 * @msg:  系统异常邮箱初始化
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_int8_t MailboxSystemExceptInit(void)
{
    rt_err_t result = RT_EOK;

    result = rt_mb_init(&MailboxSystemExcept, "MailboxSystemExcept",
                        &MailboxPool, MAX_MAIL_COUNT, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Init %s Mailbox failed!", "MailboxSystemExcept");
        return RT_ERROR;
    }
    
    return result;
}

/**
 * @name: SendMailboxSystemExcept
 * @msg:  发送系统异常邮件
 * @param [in]
 *         SendData: 待发送的数值
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SendMailboxSystemExcept(rt_uint32_t SendData)
{
    rt_mb_send(&MailboxSystemExcept, SendData);
}

/**
 * @name: RecvMailboxSystemExcept
 * @msg:  接收系统异常邮件
 * @param [in]
 *         TimeOut：超时时间
 * @param [out]
 *         *RecvData: 
 * @return the operation status, RT_EOK on successful
 * @note:  无
 */
rt_uint8_t RecvMailboxSystemExcept(rt_uint32_t* RecvData, rt_int32_t TimeOut)
{
    rt_err_t result = RT_EOK;

    result = rt_mb_recv(&MailboxSystemExcept, (rt_ubase_t *)RecvData, TimeOut);
    if (result != RT_EOK)
    {
        return RT_ERROR;
    }
    
    return result;
}

