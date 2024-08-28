#ifndef DATA_RING_BUFFER_H
#define DATA_RING_BUFFER_H

#include <rtthread.h>
#include "ipc/ringbuffer.h"
#include "Protocol_Common.h"
#include "VerificationAlgorithm.h"

#define CCM_RAM  __attribute__((section("ccmram")))

enum
{
    UP_LINK_RING_BUFFER,
	CACHE_RING_BUFFER,
    RING_BUFFER_NUM
};

typedef struct
{
    struct rt_ringbuffer *Ringbuffptr;
    uint8_t *Buffer;
    uint16_t size;
} Data_Buffer_t;

#define UP_LINK_RING_BUFFER_SIZE        (10240)
#define CACH_RING_BUFFER_SIZE           UP_LINK_RING_BUFFER_SIZE

#define DATA_BUFFER_UP                  (0)


/********Rinbuffer中存放的数据格式*******************/
#define RINGBUFFER_MESSAGE_LENGTH_BYTE         (2)
#define RINGBUFFER_MESSAGE_FUNCID_BYTE         (2)
#define RINGBUFFER_MESSAGE_ROUTE_COUNTER_BYTE  (1)
#define RINGBUFFER_MESSAGE_CHECK_BYTE          (2)

/********Rinbuffer中各字段的偏移****************/
// 占2字节
#define RINGBUFFER_MESSAGE_LENGTH_OFFSET        (0)
// 占2字节
#define RINGBUFFER_MESSAGE_FUNCID_OFFSET        (2)
#define RINGBUFFER_MESSAGE_ROUTE_COUNTER_OFFSET (4)

extern void Data_Buffer_Init(void);
extern rt_bool_t CheckRingBuffFull(struct rt_ringbuffer *RingBuffer);
extern rt_size_t Data_Buffer_Put(rt_uint8_t DataBuffer_ID,const rt_uint8_t *DataBuffer, rt_size_t DataLen);
extern rt_bool_t Data_Buffer_Is_Empty(rt_uint8_t DataBuffer_ID);
extern rt_bool_t GetMessageFromRingbuf(rt_uint8_t DataBuffer_ID,rt_uint8_t *DataBuffer, rt_uint16_t *DataLen, rt_uint16_t * FuncID);
extern rt_bool_t GetSpecifiedMessagesFromRingBuf(rt_uint8_t DataBuffer_ID,rt_uint16_t FuncID, rt_uint8_t MessageCount, rt_uint8_t *DataBuffer, rt_uint16_t *DataLen);
extern rt_bool_t Data_Buffer_Insert_Data(rt_uint8_t DataBuffer_ID,const rt_uint8_t *DataBuffer, rt_uint16_t DataLen);
extern rt_size_t CopyDataFromRingbuff(rt_uint8_t DataBuffer_ID, rt_uint8_t *DataBuffer, rt_size_t DataLen);
extern Data_Buffer_t Data_Buffer_table[RING_BUFFER_NUM];
extern rt_uint8_t GetRingbufferTypeSize(void);
extern rt_bool_t CopyFirstMessageIDFromRingbuf(rt_uint8_t DataBuffer_ID, rt_uint16_t *FuncID);
extern rt_bool_t ClearFirstMessageFromRingbuf(rt_uint8_t DataBuffer_ID, const rt_uint16_t FuncID);
#endif
