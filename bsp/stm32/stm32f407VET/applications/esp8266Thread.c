#include "esp8266Thread.h"
#include <at_device_esp8266.h>
#include <stdio.h>
#include <at_log.h>
#include "oled.h"
#include "ds18B20.h"

ALIGN(RT_ALIGN_SIZE);
static char Esp8266Thread_stack[4096];
static struct rt_thread Esp8266Thread_Thread;
esp8266NetdevInfoStruct esp8266NetdevInfo;

esp8266ConfigInfoStruct esp8266ConfigInfo;
WLANConfigNetSwitchTypeDef gWLANConfigNetFlag = STOP_WLAN_CONFIG_NET;
/**
 * @name: GetAPConnectionFlag
 * @msg:  获得WLAN连接AP成功的标志
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int8_t GetAPConnFlag(void)
{
    return ESP8266GetAPConnFlag();
}

/**
 * @name: GetAPSSID
 * @msg:  获得WLAN连接到AP的SSID信息
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return AP的SSID指针
 * @note:  无
 */
const rt_uint8_t *GetAPSSID(void)
{
    return (const rt_uint8_t *)ESP8266GetSSID();
}


/**
 * @name: GetAPMAC
 * @msg:  获得WLAN连接到AP的MAC信息
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return AP的MAC指针
 * @note:  无
 */
const rt_uint8_t *GetAPMAC(void)
{
    return (const rt_uint8_t *)ESP8266GetMAC();
}

/**
 * @name: GetAPRSSI
 * @msg:  获得WLAN连接到AP的RSSI信息
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int32_t GetAPRSSI(void)
{
    return ESP8266GetRSSI();
}

/**
 * @name: WLANDisconnWithAP
 * @msg:  WLAN断开与AP连接
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int32_t WLANDisconnWithAP(void)
{
    return ESP8266DisconnWithAP();
}

/**
 * @name: WLANReconnWithAP
 * @msg:  WLAN重连AP，本质为复位wifi
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int32_t WLANReconnWithAP(void)
{
    return ESP8266ReconnWithAP();
}

/**
 * @name: QueryWLANConnInfo
 * @msg:  查询WLAN网络连接信息
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
* @note:  WLAN需要处于Station状态，当WLAN状态为CONNECT_AP时，等待几秒后，其状态会变为DISCONNECT_NETWORK,当连上服务器会变为ESTABLISH_NETWORK，否则一直维持DISCONNECT_NETWORK状态
 */
rt_int32_t QueryWLANConnInfo(void)
{
    return ESP8266QueryNetConnInfo();
}

/**
 * @name: SetWLANSingleConn
 * @msg:  设置WLAN为单连接模式
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int32_t SetWLANSingleConn(void)
{
    return ESP8266SetSingleConn();
}

/**
 * @name: SetWLANMultiConn
 * @msg:  设置WLAN为多连接模式
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
rt_int32_t SetWLANMultiConn(void)
{
    return ESP8266SetMultiConn();
}

//SmartConfig 是一种用于将 WiFi 配置推送到 ESP8266 的技术，通常用于简化无线设备的配置过程。
//用户可以通过手机应用来将 WiFi 网络的配置信息发送到 ESP8266，使其连接到指定的 WiFi 网络。
/**
 * @name: StartWLANSmartConfig
 * @msg:  开启WLAN SmartConfig
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  1)该接口函数需要和StopWLANSmartConfig配合使用
 *         2)仅支持在WLAN单Station模式下调?;
 *         3)开启SmartConfig功能后默认断开AP连接。
 */
rt_int32_t StartWLANSmartConfig(void)
{
    return ESP8266StartSmartConfig();
}

/**
 * @name: StopWLANSmartConfig
 * @msg:  停?WLAN SmartConfig
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  1)该接口函数需要和StartWLANSmartConfig配合使用
 */
rt_int32_t StopWLANSmartConfig(void)
{
    return ESP8266StopSmartConfig();
}


/**
 * @name: SetWLANConfigNetFlag
 * @msg:  设置WLAN配网标志
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SetWLANConfigNetFlag(WLANConfigNetSwitchTypeDef SetValue)
{
    gWLANConfigNetFlag = SetValue;
}

/**
 * @name: GetWLANConfigNetFlag
 * @msg:  获得WLAN配网标志
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
WLANConfigNetSwitchTypeDef GetWLANConfigNetFlag(void)
{
    return gWLANConfigNetFlag;
}

/**
 * @name: SetWLANDNS
 * @msg:  设置WLAN的DNS
 * @param [in]
 *         WLANDNSTye： DNS类型（默认DNS或者自定义DNS）
 *         pWLANDNSAddr： 若DNS类型为自定义DNS，那么需要该参数
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SetWLANDNS(ESP8266DNSType WLANDNSTye, rt_uint8_t *pWLANDNSAddr)
{
    ESP8266SetDNS(WLANDNSTye, (char *)pWLANDNSAddr);

    return ;
}

/**
 * @name: SetWLANIP
 * @msg:  设置WLAN的IP地址
 * @param [in]
 *         WLANIPType: IP类型（静态IP或者动态IP）
 *         pWLANStaticIPInfo 若IP类型地址为静态IP，那么需要该参数
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SetWLANIP(ESP8266IPType WLANIPType, ESP8266StaticIPInfo *pWLANStaticIPInfo)
{
    ESP8266SetIP(WLANIPType, pWLANStaticIPInfo);

    return ;
}

/**
 * @name: SetWLANInfo
 * @msg:  设置WLAN的配网信息，包括SSID和Password
 * @param [in]
 *         *pWLANInfo: WLAN的配网信息(SSID和Password)
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SetWLANInfo(ESP8266WLANInfo *pWLANInfo)
{
    ESP8266SetWLANInfo(pWLANInfo);
    return ;
}

/**
 * @name: UpdateWLANInfo
 * @msg:  更新连接WLAN的信息
 * @param [in]
 *         pWLANSSID: WLAN_SSID
 *         pWLANPWD: WLAN密码
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void UpdateWLANInfo(rt_uint8_t *pWLANSSID, rt_uint8_t *pWLANPWD)
{
    ESP8266UpdateWLANInfo((char *)pWLANSSID, (char *)pWLANPWD);
//	ESP8266UpdateWLANInfo("TP", "td88888888");
    return ;
}

/**
 * @name: WLANDevClassRegister
 * @msg:  WLAN设备类注册接口
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void WLANDevClassRegister(void)
{
    ESP8266DevClassRegister();
//	rt_kprintf("into WLANDevClassRegister\n");
    return ;
}

/**
 * @name: WLANDevRegister
 * @msg:  WLAN设备注册接口
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void WLANDevRegister(void)
{
    ESP8266DevRegister();
//	rt_kprintf("into WLANDevRegister\n");
    return ;
}


static void getEsp8266NetInfo(void)
{
	char buf_ip[30] = {0}; 
	char buf_mac[30] = {0}; 
	char buf_netmask[20] = {0}; 
	char buf_gateway[20] = {0}; 
	char buf_dns_server1[20] = {0}; 
	rt_kprintf("ip:%s\n",esp8266NetdevInfo.ip);
	rt_kprintf("mac:%s\n",esp8266NetdevInfo.mac);
	rt_kprintf("netmask:%s\n",esp8266NetdevInfo.netmask);
	rt_kprintf("gateway:%s\n",esp8266NetdevInfo.gateway);
	rt_kprintf("dns_servers:%s\n",esp8266NetdevInfo.dns_server1);
	rt_sprintf(buf_ip,"ip:%s",esp8266NetdevInfo.ip);
	rt_sprintf(buf_mac,"mac:%s",esp8266NetdevInfo.mac);
	rt_sprintf(buf_netmask,"nk:%s",esp8266NetdevInfo.netmask);
	rt_sprintf(buf_gateway,"gy:%s",esp8266NetdevInfo.gateway);
//	rt_sprintf(buf_dns_server1,"sr:%s",esp8266NetdevInfo.dns_server1);
	OLED_Clear(OLED_BLACK);
	OLED_SetText(0, 0, buf_ip, 0, FONT_12X12);
	OLED_SetText(0, 16, buf_mac, 0, FONT_12X12);
	OLED_SetText(0, 32, buf_netmask, 0, FONT_12X12);
	OLED_SetText(0, 48, buf_gateway, 0, FONT_12X12);
//	OLED_SetText(0, 64, buf_dns_server1, 1, FONT_12X12);

}

static void Esp8266Thread(void *param)
{
	if(GetSemNetInfo(10000) == RT_EOK)
	getEsp8266NetInfo();
	DS18B20_ReadRomInfo();
	while(1)
	{
		DS18B20_GetTemperature();
		rt_kprintf("1S thread run!\n");
		rt_thread_delay(1000);
	}
}

/**
 * @name: OsTaskEsp8266Thread
 * @msg: 8266初始化
 * @param [in]
 *        ThreadPriority 线程优先级
 *        ThreadTick     线程时间片
 * @param [out]
 *         无
 * @return 无
 * @note: 无
 */
void OsTaskEsp8266Thread(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
    rt_thread_init(&Esp8266Thread_Thread,       // struct rt_thread  *thread
                   "Esp8266Thread",             // const char       *name
                   Esp8266Thread,               // void          (*entry)(void *parameter)
                   RT_NULL,                     // void          *parameter
                   &Esp8266Thread_stack[0],     // void          *stack_start
                   sizeof(Esp8266Thread_stack), // rt_uint32_t    stack_size
                   ThreadPriority,              // rt_uint8_t     priority
                   ThreadTick);                 // rt_uint32_t    tick

    rt_thread_startup(&Esp8266Thread_Thread);
}

