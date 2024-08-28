/*
 * @Descripttion: 亲机蓝牙与亲机MCU通讯协议源文件
 */
#include "Protocol_BLE.h"
#include "Data_RingBuffer.h"
#include "IPCResource.h"
#include "string.h"

//TODO: 如果后续需要获得该值，使用函数封装。
volatile MacAddrTypeDef MasterMacAddr = {0x00};
uint8_t m_Ringbuf_full_flag = 0;
typedef struct
{
    rt_uint16_t BLEMessageID;
    rt_uint16_t BLEMessageLen;
    rt_bool_t   BLEMessageAckFlag;
    rt_bool_t   BLEToServerFlag;
    //CallbackFunction
    void (*FunPtr)(const rt_uint8_t *DataBuffer, rt_uint16_t DataLen);
} BLEMessageParseTypedef;

//注意:上行协议亲机蓝牙与MCU之前通讯时，无同级应答。
static const BLEMessageParseTypedef BLEMessageUplinkTable[] =
{  
	{BLE_MESSAGE_ID_SENSOR_STATUS_Y_DATA, 160, RT_FALSE, RT_TRUE, NULL},
	{BLE_MESSAGE_ID_SMALL_SALE_DATA,47,RT_FALSE,RT_TRUE,NULL},
    {0,0,0,0,RT_NULL},
};

#define CRC_RESULT_DEFAULT (0xFFFF)

/**
 * @name: FindMessageIDIndex
 * @msg:  寻找指定ID在指令表中的索引或者下标

 * @param [in]
 *          FuncTabePtr：数据包主要信息
 *          FuncID: 待查询的指令ID
 * @param [out]
 *         无 
 * @return  
 * @note:  无
 */
static rt_uint8_t FindMessageIDIndex(const BLEMessageParseTypedef *FuncTabePtr, rt_uint16_t FuncID)
{
    rt_uint8_t index = 0x00;

    while (FuncTabePtr->BLEMessageID != 0x00)
    {
        if (FuncID == FuncTabePtr->BLEMessageID)
        {
            return index;
        }

        index++;
        FuncTabePtr++;
    }
    
    return UART_MESSAGE_ID_INDEX_INVALID;
}

/**
 * @name: PutMessageInUplinkDataBuf
 * @msg:  数据转存至上行数据缓存UplinkDataBuf中
 * @param [in]
 *        DataBuffer：蓝牙数据包Buffer指针
 *        DataLen：蓝牙数据包的长度（含1字节校验位）
 * @param [out]
 *         无 
 * @return {*}
 * @note:  1) 下行数据缓存DownlinkDataBuf中的数据包不包含协议包头（2字节）;
 *         2）第一个字节为协议伪长度（不包括2字节包头和2字节校验）;
 *         3）数据保存的格式如下：
 *            伪数据长度（不含校验位）2个字节（小端模式）+ MessageID + 路由设备数 + 路由列表 + 协议Message字段数据 + 校验位
 */
static rt_bool_t PutMessageInUplinkDataBuf(const rt_uint8_t *DataBuffer, rt_uint16_t DataLen)
{
    rt_uint16_t  MessageLen = 0;
    rt_uint8_t   index = UART_MESSAGE_ID_INDEX_INVALID;
    rt_uint16_t  MessageID = UART_MESSAGE_ID_INVALID;
    rt_uint8_t   tempBuffer[512] = {0};
    rt_uint16_t  CRCResult = CRC_RESULT_DEFAULT;
    rt_uint16_t  ProtocolKeyIndex = 0x0000;
	
    MessageID = (*(DataBuffer + MESSAGE_INSTRUCTION_ID_OFFSET)) + \
                (*(DataBuffer + (MESSAGE_INSTRUCTION_ID_OFFSET+1)) << 8);
    
    index = FindMessageIDIndex(BLEMessageUplinkTable, MessageID);
	rt_kprintf("index :%d!\n",index);
	
    if (index != UART_MESSAGE_ID_INDEX_INVALID)
    {
		if(MessageID == 0xB500)
		{
			/*小型机销售数据打包*/
			tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET] = 0;
			tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET + 1] = 0;
			MessageLen += RINGBUFFER_MESSAGE_LENGTH_BYTE;
			ProtocolKeyIndex += RINGBUFFER_MESSAGE_LENGTH_BYTE;
			tempBuffer[RINGBUFFER_MESSAGE_FUNCID_BYTE] = MessageID;
			tempBuffer[RINGBUFFER_MESSAGE_FUNCID_BYTE + 1] = MessageID >> 8;
			MessageLen += RINGBUFFER_MESSAGE_FUNCID_BYTE;
			ProtocolKeyIndex += RINGBUFFER_MESSAGE_LENGTH_BYTE;
			memcpy(&tempBuffer[ProtocolKeyIndex], &DataBuffer[6], BLEMessageUplinkTable[index].BLEMessageLen);
			MessageLen += BLEMessageUplinkTable[index].BLEMessageLen;
		}else{
			// 协议伪长度（不包括2字节包头和2字节校验）预占位，后续更新
			tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET] = 0;
			tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET + 1] = 0;
			MessageLen += RINGBUFFER_MESSAGE_LENGTH_BYTE;
			ProtocolKeyIndex += RINGBUFFER_MESSAGE_LENGTH_BYTE;

			tempBuffer[RINGBUFFER_MESSAGE_FUNCID_BYTE] = MessageID;
			tempBuffer[RINGBUFFER_MESSAGE_FUNCID_BYTE + 1] = MessageID >> 8;
			MessageLen += RINGBUFFER_MESSAGE_FUNCID_BYTE;
			ProtocolKeyIndex += RINGBUFFER_MESSAGE_FUNCID_BYTE;

			memcpy(&tempBuffer[ProtocolKeyIndex], DataBuffer + MESSAGE_ROUTE_COUNTER_OFFSET, BLEMessageUplinkTable[index].BLEMessageLen);
			MessageLen += BLEMessageUplinkTable[index].BLEMessageLen;
		}
    }
    else
    {
        rt_kprintf("Uplink data will not be stored because of invalid MassageID!\n");
        return RT_FALSE;
    } 
    
	// 填充伪长度预留位
	MessageLen = MessageLen;
	tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET] = MessageLen;
	tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET + 1] = (MessageLen >> 8);
	// 增加2字节，用于存放校验和
	CRCResult = CRC16_CCITT(&tempBuffer[RINGBUFFER_MESSAGE_LENGTH_OFFSET], (MessageLen));
	tempBuffer[MessageLen] = CRCResult;
	MessageLen++;
	tempBuffer[MessageLen] = (CRCResult >> 8);
	MessageLen++;
	rt_kprintf("20240325 test----------\n");
	for (int i = 0; i < MessageLen; i++)
        rt_kprintf("%02X ", tempBuffer[i]);
	rt_kprintf("\n");
	rt_kprintf("20240325 test----------\n");
	
	GetMutexUartRingBuf(RT_WAITING_FOREVER);
	if (!Data_Buffer_Put(DATA_BUFFER_UP, tempBuffer, MessageLen))
	{
		ReleaseMutexUartRingBuf();
		rt_kprintf("Data will not be stored because of no space in UplinkData ringbuffer! ");
		return RT_FALSE;
	}
	ReleaseMutexUartRingBuf();
	ReleaseSemParseUplinkData();
    return RT_TRUE;
}

/**
 * @name: Get_MacAddr
 * @msg:  获得亲机的Mac
 * @param [in]
 *         无
 * @param [out]
 *         *MacAddrString： 保存格式化的亲机Mac缓冲区指针 
 * @return 无
 * @note:  无
 */
void Get_MacAddr(rt_uint8_t *MacAddrString)
{
  rt_sprintf((char *)MacAddrString, "%02X:%02X:%02X:%02X:%02X:%02X", MasterMacAddr.MacAddr[5],
             MasterMacAddr.MacAddr[4], MasterMacAddr.MacAddr[3], MasterMacAddr.MacAddr[2],
             MasterMacAddr.MacAddr[1], MasterMacAddr.MacAddr[0]);
}

/**
 * @name: GetMasterMac
 * @msg:  获得亲机的Mac
 * @param [in]
 *         无
 * @param [out]
 *         MasterMacAddr： 亲机的Mac 
 * @return 无
 * @note:  无
 */
MacAddrTypeDef GetMasterMac(void)
{
  return MasterMacAddr;
}

/**
 * @name: Set_MacAddr
 * @msg:  设置亲机的Mac
 * @param [in]
 *         *MacAddr：亲机蓝牙的MAC地址指针
 * @param [out]
 *         无
 * @return 无
 * @note:  亲机MAC本质为亲机蓝牙的MAC。由于亲机MCU没有MAC，故将亲机蓝牙的MAC称作亲机MAC。
 */
void Set_MacAddr(MacAddrTypeDef *MacAddr)
{
  for (rt_uint8_t i = 0; i < 6; i++)
    MasterMacAddr.MacAddr[i] = MacAddr->MacAddr[i];
}


/**
 * @name: DecodeMasterBLEHeartbeat
 * @msg:  解析亲机蓝牙心跳数据，获得亲机的Mac地址
 * @param [in]
 *         *DataBuffer：指向数据Buffer的指针
 *         DataLen: 数据长度
 * @param [out]
 *         无
 * @return 无
 * @note:  1）传入的DataBuffer[0]为指令头0xBB；
 *         2）亲机蓝牙心跳由亲机蓝牙直接发送给亲机MCU，因此路由数为0，路由列表为Null（该字段不存在）。
 */
static void DecodeMasterBLEHeartbeat(const rt_uint8_t *DataBuffer, rt_uint16_t DataLen)
{
  rt_uint8_t tempString[20];
  MacAddrTypeDef MacAddr;
  rt_uint8_t index = 0;

  index = MESSAGE_HEAD_BYTE + MESSAGE_LENGTH_BYTE + MESSAGE_INSTRUCTION_ID_BYTE + MESSAGE_ROUTE_COUNTER_BYTE;

  //转存亲机蓝牙的MAC
  for (rt_uint8_t i = index; i < (index + MESSAGE_MAC_BYTE); i++)
    MacAddr.MacAddr[i - index] = DataBuffer[i];


  rt_memset(tempString, 0, sizeof(tempString));
  Get_MacAddr((rt_uint8_t *)tempString);
  rt_kprintf("The MacAddr of Master is: %s \n", tempString);


  //EncodeMasterBLEHeartbeatResponse();
}
/**
 * @name: VerifyMessageID
 * @msg: 检验一包蓝牙数据包内的指令ID
 * @param [in]
 *          DataBuffer：指向蓝牙数据包Buffer的指针
 * @param [out]
 *         无 
 * @return  校验结果 
 *          0 数据包正确，校验通过。
 *          other 数据校验有误
 * @note:  无
 */
static rt_bool_t VerifyMessageID(const rt_uint8_t *DataBuffer)
{
    rt_uint16_t MessageID = UART_MESSAGE_ID_INVALID;
    rt_uint8_t  index     = UART_MESSAGE_ID_INDEX_INVALID;
    rt_bool_t   RetValue  = RT_FALSE;

    MessageID = *(DataBuffer + MESSAGE_INSTRUCTION_ID_OFFSET) + 
                (*(DataBuffer + (MESSAGE_INSTRUCTION_ID_OFFSET+1)) << 8);

    index = FindMessageIDIndex(BLEMessageUplinkTable, MessageID);
    if (index != UART_MESSAGE_ID_INDEX_INVALID)
    {
        RetValue = RT_TRUE;
    }
    
    return RetValue;
}

/**
 * @name: VerifyBLEMessage
 * @msg:  校验蓝牙通过串口发送给MCU的数据包有效性
 * @param [in]
 *        *DataBuffer：指向数据Buffer的指针
 *        DataLen：数据的长度
 * @param [out]
 *        无
 * @return 蓝牙数据的校验结果
 * @note:  无
 */
static UARTMessageVerifyTypedef VerifyUartMessage(const uint8_t *DataBuffer, uint16_t DataLen)
{
  if (DataLen < UART_MESSAGE_MIN_LEN_BYTE)
  {
    return (UART_MESSAGE_INVALID_LEN);
  }

  if (RT_FALSE == CRC16_CCITT_Check(DataBuffer, DataLen))
  {
    return (UART_MESSAGE_INVALID_CHECKSUM);
  }

  return (UART_MESSAGE_OK);
}

/**
 * @name: RecvNewMessage
* @msg:  解析亲机上行蓝牙通过串口发送给MCU的数据包
 * @param [in]
 *        *DataBuffer：指向数据Buffer的指针
 *        DataLen：数据的长度
 * @param [out]
 *        无
 * @return 蓝牙数据的解析结果 （True / False）
 * @note:
 *         1）【调试功能】函数内部可开启蓝牙数据打印调试功能；
 *         2）DataBuffer可能包含多包数据，因此程序逻辑上需要多次解析；
 *         3）数据长度不能超过1024字节，不能小于20字节；
 *         4）亲机蓝牙 --------> 亲机MCU 通讯（注意通讯方向）
 */
void RecvNewMessage(rt_uint8_t *DataBuffer, rt_uint16_t DataLen)
{
    /* 校验Bluetooth数据的返回值 */
    UARTMessageVerifyTypedef VerifyResult = UART_MESSAGE_OK;
    /* 当前正在解析的蓝牙数据包的索引*/
    rt_uint16_t  CurrentIndex = 0;
    /*每一包蓝牙数据的长度*/
    rt_uint16_t  MessageLen = 0;
    rt_uint16_t  MessageID = UART_MESSAGE_ID_INVALID;
    rt_uint8_t   index = 0x00;
	#ifdef DEBUG_MODE
    /*打印接收到的蓝牙数据*/
    rt_kprintf("\n************BLE->MasterUnit****************************\n");
    rt_kprintf(" BLE->MasterUnit Len: %d \n", DataLen);
    for (int i = 0; i < DataLen; i++)
        rt_kprintf("%02X ", DataBuffer[i]);
    rt_kprintf("\n******************************************************\n");
	#endif
    /*如果没有解析完所有蓝牙数据，继续进行解析*/
    while (CurrentIndex < DataLen)
    {
        MessageID = DataBuffer[CurrentIndex + MESSAGE_INSTRUCTION_ID_OFFSET] +\
                    (DataBuffer[CurrentIndex + MESSAGE_INSTRUCTION_ID_OFFSET+1] << 8);
        MessageLen = DataBuffer[CurrentIndex + MESSAGE_LENGTH_OFFSET] +\
                    (DataBuffer[CurrentIndex + MESSAGE_LENGTH_OFFSET+1] << 8);
		
		rt_kprintf("\n***********MessageID :%04x,MessageLen :%04x**********************\n",MessageID,MessageLen);
        if (!MessageLen)
        {
            rt_kprintf("[Error]The length of data packet is invalid!\n");
            return ;
        }
		//205和206字节数不够进入VerifyBLEMessage
        //告诉上行蓝牙亲机MCU网络模式
	
        VerifyResult = VerifyUartMessage(&DataBuffer[CurrentIndex+4], MessageLen-4);
        if (UART_MESSAGE_OK == VerifyResult)
        {		
			rt_kprintf("VerifyResult is OK!\n");
            //由于VerifyBLEMessage内部已经对MessageID做了校验，因此此时只获得值即可。
            index = FindMessageIDIndex(BLEMessageUplinkTable, MessageID);
			rt_kprintf("index : %d!\n",index);
            //如果接收到上行蓝牙包指令ID为0600时不进入else判断
            if (0x0600 == MessageID)             //Uart-TX同级应答协议
            {
                rt_kprintf("Uart ACK from Master-BLE!\n");
            }
            else                                             
            {
                //第一个if没用到
                //一般为亲机自身数据
                if (BLEMessageUplinkTable[index].FunPtr != RT_NULL)
                {
                    (*BLEMessageUplinkTable[index].FunPtr)(&DataBuffer[CurrentIndex], MessageLen);
                }
                if (BLEMessageUplinkTable[index].BLEToServerFlag == RT_TRUE){
                    PutMessageInUplinkDataBuf(&DataBuffer[CurrentIndex], MessageLen);				
				}
            }
        }
        else
        {
            rt_kprintf("[---Error---]The analysis result of FuncID [%#04x] sent by Master-BLE is: %d!\n", MessageID, VerifyResult);
        }

        CurrentIndex += MessageLen;  
    } 
}

