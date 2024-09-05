
#ifndef __ESP8266THREAD_H
#define __ESP8266THREAD_H

#include <rtthread.h>
#include <rtdevice.h>
#include <rtconfig.h>
#include "IPCResource.h"

typedef struct
{
	uint8_t gAPConnectionFlag;
	uint8_t gGetNetInfoFlag;	
}esp8266ConfigInfoStruct;

typedef enum
{
    STOP_WLAN_CONFIG_NET = 0,
    START_WLAN_CONFIG_NET,
    WLAN_CONFIG_NET_SWITCH_TYPE
}WLANConfigNetSwitchTypeDef;

// 网络信息结构体
typedef struct {
    char ip[16];
    char mac[18];
    char gateway[16];
    char netmask[16];
    char dns_server1[16];
    char dns_server2[16];
} esp8266NetdevInfoStruct;

extern esp8266NetdevInfoStruct esp8266NetdevInfo;
extern esp8266ConfigInfoStruct esp8266ConfigInfo;

extern void OsTaskEsp8266Thread(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick);

#endif

