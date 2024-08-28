#include "Data_RingBuffer.h"
#include "ipc/ringbuffer.h"
#include "IPCResource.h"
CCM_RAM static rt_uint8_t Up_Link_Ring_Buffer[UP_LINK_RING_BUFFER_SIZE];
CCM_RAM static rt_uint8_t Cache_Ring_Buffer[CACH_RING_BUFFER_SIZE];

//除了下面2个ringbuff外还有其他ringbuff
static struct rt_ringbuffer RingBuffer_UpLink;
static struct rt_ringbuffer RingBuffer_Cache;
//该函数是方便上面2个ringbuff初始化
Data_Buffer_t Data_Buffer_table[RING_BUFFER_NUM] =
{
    {&RingBuffer_UpLink,    Up_Link_Ring_Buffer,    UP_LINK_RING_BUFFER_SIZE},
	{&RingBuffer_Cache,     Cache_Ring_Buffer,      CACH_RING_BUFFER_SIZE},
};

void Data_Buffer_Init(void)
{
    uint8_t i;

    // Active_Data_Buffer = DATA_BUFFER_A;
    for (i = 0; i < RING_BUFFER_NUM; i++)
    {
        rt_ringbuffer_init(Data_Buffer_table[i].Ringbuffptr, Data_Buffer_table[i].Buffer, Data_Buffer_table[i].size);
    }
}

/**
 * @name: CheckRingBuffFull
 * @msg:  检查环形缓冲区是否已写满
 * @param [in]  
 *        RingBuffer 待检查的环形缓冲区指针
 * @param [out]
 *        无
 * @return 缓冲区状态：1 -> 缓冲区已满； 0 -> 缓冲区未满。
 * @note: 
 */
rt_bool_t CheckRingBuffFull(struct rt_ringbuffer *RingBuffer)
{
    uint16_t result = 0;

    result = rt_ringbuffer_get_size(RingBuffer) - (uint16_t)rt_ringbuffer_data_len(RingBuffer);
    if (result < 1)
    {
        return RT_TRUE;
    }
    
    return RT_FALSE;
}

/**
 * @name: Data_Buffer_Is_Empty
 * @msg: 判断环形缓冲区是否有数据
 * @param 无
 * @return {*}
 * @note: RT_FALSE: 缓冲区中有数据
 *        RT_TRUE：缓冲区中无数据
 */
rt_bool_t Data_Buffer_Is_Empty(rt_uint8_t DataBuffer_ID)
{
    if (rt_ringbuffer_data_len(Data_Buffer_table[DataBuffer_ID].Ringbuffptr) > 0)
        return RT_FALSE;
    else
        return RT_TRUE;
}

/**
 * @name: Data_Buffer_Put
 * @msg: 
 * @param {const rt_uint8_t} *DataBuffer
 * @param {rt_size_t} DataLen
 * @return {*}
 * @note: 如果缓冲区没有足够内存空间，那么直接舍弃数据
 *        注意数据存放时的格式：
 */
rt_size_t Data_Buffer_Put(rt_uint8_t DataBuffer_ID, const rt_uint8_t *DataBuffer, rt_size_t DataLen)
{
    rt_size_t RetValue;

    if (rt_ringbuffer_space_len(Data_Buffer_table[DataBuffer_ID].Ringbuffptr) > DataLen)
        RetValue = rt_ringbuffer_put((Data_Buffer_table[DataBuffer_ID].Ringbuffptr), DataBuffer, DataLen);
    else
        RetValue = 0;
    return RetValue;
}

/**
 * @name: GetMessageFromRingbuf
 * @msg:  从环形缓冲区中取一包数据
 * @param [in]  
 *        DataBuffer_ID: ringbuffer编号
 * @param [out]
 *        DataBuffer：数据存放的Buffer（第一字节为伪数据长度，不含校验位）
 *        DataLen：数据长度（包含校验位）
 *        FuncID： 指令ID
 * @return RT_TRUE： 数据提取正确
 *         RT_FALSE： 数据提取错误
 * @note: 1）注意ringbuffer中数据存放的格式：MessageLen（2字节，不包含校验位） + FuncID （2字节）+ ValidData（实际数据） + 校验和（2字节）
 *        2）取出的数据格式同ringbuffer中一致；
 *        3）源buffer数据保存的格式如下：
 *           伪数据长度（不含校验位）+ MessageID + 路由设备数 + 路由列表 + 协议Message字段数据 + 校验位
 */
rt_bool_t GetMessageFromRingbuf(rt_uint8_t DataBuffer_ID, rt_uint8_t *DataBuffer, rt_uint16_t *DataLen, rt_uint16_t *FuncID)
{
    rt_uint16_t MassageLen = 0;
    rt_uint8_t  TempBuff[BLE_MESSAGE_MAX_DATA_LEN] = {0};
    rt_uint16_t ValidMassageLen = 0; 

    //获得一包数据的数据头即数据长度(不含校验位),占2位，直接使用rt_uint16_t读取长度
    if (0 == rt_ringbuffer_get(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, (rt_uint8_t *)&MassageLen, RINGBUFFER_MESSAGE_LENGTH_BYTE))
    {
        rt_kprintf("[---Error---] The length of ringbuff is 0!\n");
        return RT_FALSE;
    }
    else
    {
        ValidMassageLen = rt_ringbuffer_data_len(Data_Buffer_table[DataBuffer_ID].Ringbuffptr);
		rt_kprintf("----------------------- ValidMassageLen : %04x -----------------!\n",ValidMassageLen);
        //ringbuff已经把两个字节去除了，
        if (ValidMassageLen < (MassageLen - RINGBUFFER_MESSAGE_LENGTH_BYTE))
        {
            //清洗脏数据--分段清洗
            rt_ringbuffer_get(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff, ValidMassageLen);
            rt_kprintf("[---Error---] The lack of complete data!\n");
            return RT_FALSE;
        }
        else
        {
            //伪数据长度,不包含校验位
            *DataBuffer = MassageLen;
            *(DataBuffer + 1) = (MassageLen >>8);

            //从FuncID开始复制数据，包含校验和位
            rt_ringbuffer_get((Data_Buffer_table[DataBuffer_ID].Ringbuffptr), (DataBuffer+RINGBUFFER_MESSAGE_LENGTH_BYTE), \
                              (MassageLen-RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_CHECK_BYTE));
            // +2 数据包的最后2字节（校验位）
            // TODO: 这里校验数据的意义是什么呢？？？？？
            if (CRC16_CCITT_Check(DataBuffer, MassageLen+RINGBUFFER_MESSAGE_CHECK_BYTE))
            {
                *FuncID = (*(DataBuffer+RINGBUFFER_MESSAGE_LENGTH_BYTE)) +\
                          (*(DataBuffer+(RINGBUFFER_MESSAGE_LENGTH_BYTE+1)) << 8);
                //实际数据长度包含校验位
                *DataLen = MassageLen + RINGBUFFER_MESSAGE_CHECK_BYTE;
                return RT_TRUE;
            }
            else
            {
                *FuncID = UART_MESSAGE_ID_INVALID;
                //实际数据长度包含校验位
                *DataLen = 0;
                rt_kprintf("[---Error---] Data verification failed!\n");
                return RT_FALSE;
            }
        }
    }
}

/**
 * @name: CopyFirstMessageIDFromRingbuf
 * @msg:  从环形缓冲区中复制第一包数据的MessageID
 * @param [in]
 *        DataBuffer_ID: ringbuffer编号
 * @param [out]
 *        FuncID： 指令ID
 * @return RT_TRUE： 数据提取正确
 *         RT_FALSE： 数据提取错误
 * @note: 1）注意ringbuffer中数据存放的格式：MessageLen（2字节，不包含校验位） + FuncID （2字节）+ ValidData（实际数据） + 校验和（2字节）
 *        2）取出的数据格式同ringbuffer中一致；
 *        3）源buffer数据保存的格式如下：
 *           伪数据长度（不含校验位）+ MessageID + 路由设备数 + 路由列表 + 协议Message字段数据 + 校验位
 */
rt_bool_t CopyFirstMessageIDFromRingbuf(rt_uint8_t DataBuffer_ID, rt_uint16_t *FuncID)
{
    rt_uint8_t TempBuff[RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE] = {0};
    rt_uint8_t size = 0;
    
    //仅提取4个字节，包括：MessageLen（2字节，不包含校验位） + FuncID （2字节）
    size = rt_ringbuffer_copy(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff,\
                              RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE);
    rt_kprintf("size is %d!\n", size);
    for (int i = 0; i < sizeof(TempBuff)/sizeof(rt_uint8_t); i++)
    {
        rt_kprintf(" %d!\n", TempBuff[i]);
    }
    
    
    if(size == (RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE))
    {
        *FuncID = TempBuff[RINGBUFFER_MESSAGE_FUNCID_OFFSET] + (TempBuff[RINGBUFFER_MESSAGE_FUNCID_OFFSET+1]<<8);
        return RT_TRUE;
    }
    else
    {
        rt_kprintf("[---Error---] The length of ringbuff is insufficient!\n");
        //清除脏数据
        rt_ringbuffer_get(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff, \
                          (RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE));
        *FuncID = UART_MESSAGE_ID_INVALID;
        return RT_FALSE;
    }
}

/**
 * @name: ClearFirstMessageFromRingbuf
 * @msg:  从环形缓冲区中清除第一包数据(仅仅用于配合CopyFirstMessageIDFromRingbuf)
 * @param [in]
 *        DataBuffer_ID: ringbuffer编号
 *        FuncID： 指令ID
 * @param [out]
 *        无
 * @return RT_TRUE： 数据提取正确
 *         RT_FALSE： 数据提取错误
 * @note: 1）注意ringbuffer中数据存放的格式：MessageLen（2字节，不包含校验位） + FuncID （2字节）+ ValidData（实际数据） + 校验和（2字节）
 *        2）取出的数据格式同ringbuffer中一致；
 *        3）源buffer数据保存的格式如下：
 *           伪数据长度（不含校验位）+ MessageID + 路由设备数 + 路由列表 + 协议Message字段数据 + 校验位
 */
rt_bool_t ClearFirstMessageFromRingbuf(rt_uint8_t DataBuffer_ID, const rt_uint16_t FuncID)
{
    rt_uint8_t  TempBuff[10] = {0};
    rt_uint16_t ClearMassageLen = 0;
    
    //仅提取4个字节，包括：MessageLen（2字节，不包含校验位） + FuncID （2字节）
    if((RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE) == rt_ringbuffer_copy(\
        Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff, \
        (RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE)))
    {
        if (FuncID == (TempBuff[RINGBUFFER_MESSAGE_FUNCID_OFFSET] + (TempBuff[RINGBUFFER_MESSAGE_FUNCID_OFFSET+1]<<8)))
        {
            //计算该包数据长度，其中+2是校验位
            ClearMassageLen = TempBuff[RINGBUFFER_MESSAGE_LENGTH_OFFSET] + (TempBuff[RINGBUFFER_MESSAGE_LENGTH_OFFSET+1]<<8)\
                              + RINGBUFFER_MESSAGE_CHECK_BYTE;
            rt_uint16_t size = rt_ringbuffer_data_len(Data_Buffer_table[DataBuffer_ID].Ringbuffptr);
            if(size < ClearMassageLen)
            {
                ClearMassageLen = size;
            }
            
            //清除该包数据
            for (int i = 0; i < ClearMassageLen/(sizeof(TempBuff)/sizeof(rt_uint8_t)); i++)
            {
                rt_memset(TempBuff, 0, sizeof(TempBuff));
                rt_ringbuffer_get(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff, (sizeof(TempBuff)/sizeof(rt_uint8_t)));
            }
            
            rt_uint16_t remainder = ClearMassageLen%(sizeof(TempBuff)/sizeof(rt_uint8_t));
            for(int j = 0; j < remainder; j++)
            {
                rt_memset(TempBuff, 0, sizeof(TempBuff));
                rt_ringbuffer_getchar(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff);
            }
        }
        else
        {
            return RT_FALSE;
        }
    }
    else
    {
        rt_ringbuffer_get(Data_Buffer_table[DataBuffer_ID].Ringbuffptr, TempBuff, \
                          (RINGBUFFER_MESSAGE_LENGTH_BYTE+RINGBUFFER_MESSAGE_FUNCID_BYTE));
        return RT_FALSE;
    }

    return RT_TRUE;
}

static rt_bool_t Data_Buffer_Copy(rt_uint8_t DataBuffer_Dest, rt_uint8_t DataBuffer_Src)
{
    rt_size_t DataLen = 0;
    rt_uint8_t * DataPtr = RT_NULL;

    DataLen = rt_ringbuffer_data_len(Data_Buffer_table[DataBuffer_Src].Ringbuffptr);
    if (0 == DataLen)
    {
        rt_kprintf("The ringbuffer is empty!\n");
        return RT_FALSE;
    }
    
    DataPtr = rt_malloc(DataLen);
    if(DataPtr == RT_NULL)
    {
        rt_kprintf("The malloc operation failed!\n");
        return RT_FALSE;
    }
    
    rt_ringbuffer_get(Data_Buffer_table[DataBuffer_Src].Ringbuffptr, DataPtr, DataLen);
    
    //TODO: 如果需要写其他的Ringbuffer，那么首先需要定义对应的互斥量，然后在此处参照如下代码添加else if分支
    if (&RingBuffer_UpLink == Data_Buffer_table[DataBuffer_Dest].Ringbuffptr)
    {
        GetMutexUartRingBuf(RT_WAITING_FOREVER);
        rt_ringbuffer_put(Data_Buffer_table[DataBuffer_Dest].Ringbuffptr, DataPtr, DataLen);
        ReleaseMutexUartRingBuf();
    }
    
    rt_free(DataPtr);
    return RT_TRUE;
}

/**
 * @name: GetSpecifiedMessagesFromRingBuf
 * @msg:  从ringbuffer中提取若干条和FuncID指令相同的数据
 * @param [in]  
 *        DataBuffer_ID： 提前数据的buffer对象
 *        FuncID： 指令ID
 * @param [out]
 *        MessageCount：最大提取相同FuncID指令的个数
 *        DataBuffer：数据存放的Buffer
 *        DataLen：   数据长度
 * @return RT_TRUE：  提取到至少一条数据
 *         RT_FALSE： 没有相同的数据
 * @note: 无
 */
rt_bool_t GetSpecifiedMessagesFromRingBuf(rt_uint8_t DataBuffer_ID, rt_uint16_t FuncID, rt_uint8_t MessageCount, rt_uint8_t *DataBuffer, rt_uint16_t *DataLen)
{
    rt_uint8_t  tempBuffer[BLE_MESSAGE_MAX_DATA_LEN] = {0};
    rt_uint16_t MessageSize = 0;
    rt_uint16_t Current_FuncID = UART_MESSAGE_ID_INVALID;
    rt_uint8_t  MessageCounter = 0;
    rt_uint8_t  *DataPtr = RT_NULL;
    rt_bool_t   RetValue = RT_FALSE;

    DataPtr = DataBuffer;

    if ((0 == MessageCount) || Data_Buffer_Is_Empty(DataBuffer_ID))
    {
        *DataLen = 0;
        return RetValue;
    }
    
    while ((MessageCounter < MessageCount) && GetMessageFromRingbuf(DataBuffer_ID, tempBuffer, &MessageSize, &Current_FuncID))
    {
        if (FuncID == Current_FuncID)
        {
            rt_memcpy(DataPtr, tempBuffer, MessageSize);
            DataPtr += MessageSize;
            MessageCounter++;
            RetValue = RT_TRUE;
        }
        else
        {
            //数据存放到备用缓冲区中
            Data_Buffer_Put(CACHE_RING_BUFFER, tempBuffer, MessageSize);
        }

        rt_memset(tempBuffer, 0, sizeof(tempBuffer));
        if (Data_Buffer_Is_Empty(DataBuffer_ID))
        {
            break;
        }
    }

    *DataLen = MessageSize * MessageCounter;
    
    //将备用缓冲区中的数据重新放回Ringbuffer中
    //可能UpLinkRingbuffer没有被遍历完全，因此把剩余的数据也转存到备份缓冲区中
    Data_Buffer_Copy( CACHE_RING_BUFFER , DataBuffer_ID);
    Data_Buffer_Copy( DataBuffer_ID , CACHE_RING_BUFFER);

    return RetValue;
}

/**
 * @name: Data_Buffer_Insert_Data
 * @msg: 将数据放回ringbuff
 * @param [in]  
 *        DataBuffer：数据存放的Buffer
 *        DataLen：数据长度存放的地址
 * @param [out]
 *        无
 * @return 
 * @note: 放回的数据必须保证在读指针邻近前面，不允许在调整顺序的时候，插入新数据。  
 *        可以考虑使用互斥量保护。待信号量解除后，新进数据会在写指针的最新位置。
 *        暂时不考虑ringbuffer溢出的情况
 *       
 */
rt_bool_t  Data_Buffer_Insert_Data(rt_uint8_t DataBuffer_ID,const rt_uint8_t *DataBuffer, rt_uint16_t DataLen)
{
    rt_bool_t RetValue = RT_TRUE;
    rt_uint16_t BufferDataSize;
    rt_uint8_t tempData;

    BufferDataSize = rt_ringbuffer_data_len(Data_Buffer_table[DataBuffer_ID].Ringbuffptr);

    if(DataLen < (Data_Buffer_table[DataBuffer_ID].size - BufferDataSize))
    {
        if( rt_ringbuffer_put((Data_Buffer_table[DataBuffer_ID].Ringbuffptr), DataBuffer, DataLen))
        {
            //解决顺序问题，使返回的数据始终在读指针前面。
            //此处需要【互斥量保护】，暂不允许新数据写入。
            while(BufferDataSize)
            {
                rt_ringbuffer_getchar((Data_Buffer_table[DataBuffer_ID].Ringbuffptr), &tempData );
                // rt_ringbuffer_putchar((Data_Buffer_table[Active_Data_Buffer].Ringbuffptr), &tempData);
                rt_ringbuffer_putchar((Data_Buffer_table[DataBuffer_ID].Ringbuffptr), tempData);
                
                BufferDataSize--;
            }
            RetValue = RT_TRUE;
        }
        else
            RetValue = RT_FALSE;
    }
    else
        RetValue = RT_FALSE;

    return RetValue;
}

/**
 * @name: CopyDataFromRingbuff
 * @msg:  从指令的Ringbuffer中复制若干数据
 * @param [in]
 *         DataBuffer_ID: 指定的Ringbuffer;
 *         DataBuffer：数据缓存地址
 *         DataLen： 复制的数据个数
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_size_t CopyDataFromRingbuff(rt_uint8_t DataBuffer_ID, rt_uint8_t *DataBuffer, rt_size_t DataLen)
{
    rt_size_t RetValue;

    if (rt_ringbuffer_space_len(Data_Buffer_table[DataBuffer_ID].Ringbuffptr) > DataLen)
        RetValue = rt_ringbuffer_copy((Data_Buffer_table[DataBuffer_ID].Ringbuffptr), DataBuffer, DataLen);
    else
        RetValue = 0;
    return RetValue;
}

/**
 * @name: GetRingbufferTypeSize
 * @msg:  计算Ringbuffer类型大小
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_uint8_t GetRingbufferTypeSize(void)
{
    return sizeof(RingBuffer_UpLink);
}






