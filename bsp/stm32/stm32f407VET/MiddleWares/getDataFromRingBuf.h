#ifndef GETDATA_FROM_RINGBUF_H
#define GETDATA_FROM_RINGBUF_H

#include <rtthread.h>


rt_bool_t GetMessagesFromUplinkRingBuf(rt_uint8_t *DataBuffer, rt_uint16_t *DataLen);
rt_bool_t Server_Decode_Message(const rt_uint8_t *DataBuffer, rt_uint8_t *Response);
#endif


