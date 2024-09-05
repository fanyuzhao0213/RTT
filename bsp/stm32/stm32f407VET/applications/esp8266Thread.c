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
 * @msg:  ���WLAN����AP�ɹ��ı�־
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int8_t GetAPConnFlag(void)
{
    return ESP8266GetAPConnFlag();
}

/**
 * @name: GetAPSSID
 * @msg:  ���WLAN���ӵ�AP��SSID��Ϣ
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return AP��SSIDָ��
 * @note:  ��
 */
const rt_uint8_t *GetAPSSID(void)
{
    return (const rt_uint8_t *)ESP8266GetSSID();
}


/**
 * @name: GetAPMAC
 * @msg:  ���WLAN���ӵ�AP��MAC��Ϣ
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return AP��MACָ��
 * @note:  ��
 */
const rt_uint8_t *GetAPMAC(void)
{
    return (const rt_uint8_t *)ESP8266GetMAC();
}

/**
 * @name: GetAPRSSI
 * @msg:  ���WLAN���ӵ�AP��RSSI��Ϣ
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int32_t GetAPRSSI(void)
{
    return ESP8266GetRSSI();
}

/**
 * @name: WLANDisconnWithAP
 * @msg:  WLAN�Ͽ���AP����
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int32_t WLANDisconnWithAP(void)
{
    return ESP8266DisconnWithAP();
}

/**
 * @name: WLANReconnWithAP
 * @msg:  WLAN����AP������Ϊ��λwifi
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int32_t WLANReconnWithAP(void)
{
    return ESP8266ReconnWithAP();
}

/**
 * @name: QueryWLANConnInfo
 * @msg:  ��ѯWLAN����������Ϣ
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
* @note:  WLAN��Ҫ����Station״̬����WLAN״̬ΪCONNECT_APʱ���ȴ��������״̬���ΪDISCONNECT_NETWORK,�����Ϸ��������ΪESTABLISH_NETWORK������һֱά��DISCONNECT_NETWORK״̬
 */
rt_int32_t QueryWLANConnInfo(void)
{
    return ESP8266QueryNetConnInfo();
}

/**
 * @name: SetWLANSingleConn
 * @msg:  ����WLANΪ������ģʽ
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int32_t SetWLANSingleConn(void)
{
    return ESP8266SetSingleConn();
}

/**
 * @name: SetWLANMultiConn
 * @msg:  ����WLANΪ������ģʽ
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
rt_int32_t SetWLANMultiConn(void)
{
    return ESP8266SetMultiConn();
}

//SmartConfig ��һ�����ڽ� WiFi �������͵� ESP8266 �ļ�����ͨ�����ڼ������豸�����ù��̡�
//�û�����ͨ���ֻ�Ӧ������ WiFi �����������Ϣ���͵� ESP8266��ʹ�����ӵ�ָ���� WiFi ���硣
/**
 * @name: StartWLANSmartConfig
 * @msg:  ����WLAN SmartConfig
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  1)�ýӿں�����Ҫ��StopWLANSmartConfig���ʹ��
 *         2)��֧����WLAN��Stationģʽ�µ�?;
 *         3)����SmartConfig���ܺ�Ĭ�϶Ͽ�AP���ӡ�
 */
rt_int32_t StartWLANSmartConfig(void)
{
    return ESP8266StartSmartConfig();
}

/**
 * @name: StopWLANSmartConfig
 * @msg:  ͣ?WLAN SmartConfig
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  1)�ýӿں�����Ҫ��StartWLANSmartConfig���ʹ��
 */
rt_int32_t StopWLANSmartConfig(void)
{
    return ESP8266StopSmartConfig();
}


/**
 * @name: SetWLANConfigNetFlag
 * @msg:  ����WLAN������־
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
void SetWLANConfigNetFlag(WLANConfigNetSwitchTypeDef SetValue)
{
    gWLANConfigNetFlag = SetValue;
}

/**
 * @name: GetWLANConfigNetFlag
 * @msg:  ���WLAN������־
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
WLANConfigNetSwitchTypeDef GetWLANConfigNetFlag(void)
{
    return gWLANConfigNetFlag;
}

/**
 * @name: SetWLANDNS
 * @msg:  ����WLAN��DNS
 * @param [in]
 *         WLANDNSTye�� DNS���ͣ�Ĭ��DNS�����Զ���DNS��
 *         pWLANDNSAddr�� ��DNS����Ϊ�Զ���DNS����ô��Ҫ�ò���
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
void SetWLANDNS(ESP8266DNSType WLANDNSTye, rt_uint8_t *pWLANDNSAddr)
{
    ESP8266SetDNS(WLANDNSTye, (char *)pWLANDNSAddr);

    return ;
}

/**
 * @name: SetWLANIP
 * @msg:  ����WLAN��IP��ַ
 * @param [in]
 *         WLANIPType: IP���ͣ���̬IP���߶�̬IP��
 *         pWLANStaticIPInfo ��IP���͵�ַΪ��̬IP����ô��Ҫ�ò���
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
void SetWLANIP(ESP8266IPType WLANIPType, ESP8266StaticIPInfo *pWLANStaticIPInfo)
{
    ESP8266SetIP(WLANIPType, pWLANStaticIPInfo);

    return ;
}

/**
 * @name: SetWLANInfo
 * @msg:  ����WLAN��������Ϣ������SSID��Password
 * @param [in]
 *         *pWLANInfo: WLAN��������Ϣ(SSID��Password)
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
void SetWLANInfo(ESP8266WLANInfo *pWLANInfo)
{
    ESP8266SetWLANInfo(pWLANInfo);
    return ;
}

/**
 * @name: UpdateWLANInfo
 * @msg:  ��������WLAN����Ϣ
 * @param [in]
 *         pWLANSSID: WLAN_SSID
 *         pWLANPWD: WLAN����
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
void UpdateWLANInfo(rt_uint8_t *pWLANSSID, rt_uint8_t *pWLANPWD)
{
    ESP8266UpdateWLANInfo((char *)pWLANSSID, (char *)pWLANPWD);
//	ESP8266UpdateWLANInfo("TP", "td88888888");
    return ;
}

/**
 * @name: WLANDevClassRegister
 * @msg:  WLAN�豸��ע��ӿ�
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
 */
void WLANDevClassRegister(void)
{
    ESP8266DevClassRegister();
//	rt_kprintf("into WLANDevClassRegister\n");
    return ;
}

/**
 * @name: WLANDevRegister
 * @msg:  WLAN�豸ע��ӿ�
 * @param [in]
 *         ��
 * @param [out]
 *         ��
 * @return ��
 * @note:  ��
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
 * @msg: 8266��ʼ��
 * @param [in]
 *        ThreadPriority �߳����ȼ�
 *        ThreadTick     �߳�ʱ��Ƭ
 * @param [out]
 *         ��
 * @return ��
 * @note: ��
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

