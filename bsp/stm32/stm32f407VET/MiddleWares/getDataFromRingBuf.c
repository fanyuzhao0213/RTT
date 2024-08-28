#include "getDataFromRingBuf.h"
#include "Data_RingBuffer.h"


typedef struct 
{
    rt_uint16_t  Message_Id;					//ID
    rt_uint16_t  Message_Size;					//size
    rt_uint8_t   Message_Counter;				//消息个数，类似于有几条
    //函数指针
    rt_bool_t (*FunPtr)(const rt_uint8_t *DataBuffer, rt_uint8_t *Response);

}Server_Parse_t;

static rt_bool_t UploadSensorStatusY(const uint8_t *DataBuffer, uint8_t *Response);
static rt_bool_t UploadSmallMachineSale(const uint8_t *DataBuffer, uint8_t *Response);

#define COMMAND_TYPE_COUNT    2

//TODO: 根据表中的条目需要动态修改该值
Server_Parse_t Server_Parse_Table[COMMAND_TYPE_COUNT] =
{
	{BLE_MESSAGE_ID_SENSOR_STATUS_Y_DATA, BLE_MESSAGE_SENSOR_STATUS_Y_DATA_LEN, 1, UploadSensorStatusY},
	{BLE_MESSAGE_ID_SMALL_SALE_DATA,BLE_MESSAGE_SENSOR_SALE_DATA_LEN,1,UploadSmallMachineSale},
};

static rt_bool_t UploadSensorStatusY(const uint8_t *DataBuffer, uint8_t *Response)
{
	rt_kprintf("UploadSensorStatusY !\n");
}

static rt_bool_t UploadSmallMachineSale(const uint8_t *DataBuffer, uint8_t *Response)
{
	rt_kprintf("UploadSmallMachineSale !\n");
}


rt_bool_t Server_Check_FuncID(rt_uint16_t FuncID)
{
    for(rt_uint8_t i = 0; i < 2; i++)
    {
        if(FuncID == Server_Parse_Table[i].Message_Id)
            return RT_TRUE;
    }

    return RT_FALSE;
}

rt_uint8_t FindMessageCounter(rt_uint16_t FuncID)
{
    for(rt_uint8_t i = 0; i < 2; i++)
    {
        if(FuncID == Server_Parse_Table[i].Message_Id)
            return Server_Parse_Table[i].Message_Counter;
    }

    return 0;
}


/**
 * @name: GetMessagesFromUplinkRingBuf
 * @msg:  从UplinkRingBuf中获得多包数据
 * @param [in]
 *         无
 * @param [out]
 *         *DataBuffer：转存数据的buffer
 *         *DataLen：转存数据长度
 * @return 无
 * @note:  无
 */
rt_bool_t GetMessagesFromUplinkRingBuf(rt_uint8_t *DataBuffer, rt_uint16_t *DataLen)
{
    rt_uint8_t  TempBuffer[1024] = {0};
    rt_uint16_t  MessageID = 0;
    rt_uint16_t MessageSize = 0;
    rt_uint16_t DataIndex = 0;
    rt_bool_t   RetValue = RT_FALSE;
    if (GetMessageFromRingbuf(DATA_BUFFER_UP, TempBuffer, &MessageSize, &MessageID))
    // if (CopyFirstMessageIDFromRingbuf(DATA_BUFFER_UP, &MessageID))
    {
        DataIndex += MessageSize;
        if (Server_Check_FuncID(MessageID))
        {
            rt_uint8_t MessageCounter = FindMessageCounter(MessageID);
            MessageCounter--;
            rt_memcpy(DataBuffer, TempBuffer, DataIndex);
            *DataLen = DataIndex;
            RetValue = RT_TRUE;
        }
        else
        {
            rt_kprintf("The MessageID of packet from Ringbuffer is invalid!\n");
            // ClearFirstMessageFromRingbuf(DATA_BUFFER_UP, MessageID);
            RetValue = RT_FALSE;
        }
    }

    return RetValue;
}

/**
 * @name: Server_Decode_Message
 * @msg:  打包并发送需要上传到服务器的数据
 * @param [in]
 *        DataBuffer：数据包Buffer指针
 * @param [out]
 *        Response：服务器的响应信息
 * @return 
 * @note: 数据包中虽然含有若干包数据，但是他们的指令ID都相同，属于同类型的指令。
 */
rt_bool_t Server_Decode_Message(const rt_uint8_t *DataBuffer, rt_uint8_t *Response)
{
    rt_uint16_t FuncID = UART_MESSAGE_ID_INVALID;
    rt_uint8_t  index = 0;
    rt_bool_t   RetValue = RT_FALSE;

    FuncID = DataBuffer[RINGBUFFER_MESSAGE_FUNCID_BYTE] + \
             (DataBuffer[RINGBUFFER_MESSAGE_FUNCID_BYTE+1]<<8);

    for (; index < COMMAND_TYPE_COUNT; index++)
    {
        if(Server_Parse_Table[index].Message_Id == FuncID)
        {
            if (Server_Parse_Table[index].FunPtr != RT_NULL)
                RetValue = Server_Parse_Table[index].FunPtr(DataBuffer, Response);
            else
            {
                rt_kprintf("Please add FunPtr about %#02X in Server_Parse_Table!\n", Server_Parse_Table[index].Message_Id);
                RetValue = RT_FALSE;
            }
            break;
        }
        else
            continue; 
    }

    if (COMMAND_TYPE_COUNT == index)
    {
        //TODO: 不处理脏数据么？？？
        rt_kprintf("Please add MessageId about %#02X in Server_Parse_Table!\n", FuncID);
        RetValue = RT_FALSE;
    }
    return RetValue;
}
