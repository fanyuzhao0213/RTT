/*
 * @Descripttion: 亲机蓝牙与亲机MCU通讯协议头文件
 */
#ifndef PROTOCOL_BLE_H
#define PROTOCOL_BLE_H

#include "Protocol_Common.h"
#include "EEPROM.h"
#pragma anon_unions
#pragma pack(push, 1)
typedef struct
{
    uint8_t g_upble_mac[6];
    uint8_t g_downble_mac_01[6];
    uint8_t g_downble_mac_02[6];
    uint8_t g_get_upblemac_flag;
    uint8_t g_get_downblemac_flag;
} GetUpDownMacTypeDef;

typedef struct
{
	uint8_t MacAddr[MESSAGE_MAC_BYTE];
} MacAddrTypeDef;

extern GetUpDownMacTypeDef getUpDownMacTypeDef;

typedef struct{
	char pol[2];
	char k_sts[1];
	char k_kno[2];
	char k_gu[1];
	char k_uk[1];
	char k_gt[3];
	char c_id[6];
	char c_po[5];
	char c_mn[4];
	char k_uc1[4];
	char k_uc2[4];
	char k_uc3[4];
	char k_uc4[4];
	char k_uc5[4];
	char k_gk[2];
	char k_er[2];
	char st_01[3];
	char st_02[2];
	char st_03[2];
	char st_04[12];
	char st_05[3];
	char st_06[3];
	char st_07[4];
	char st_08[3];
	char st_09[4];
	char st_10[3];
}MachineStatusTypeDef;

typedef union
{
	struct
	{
		MacAddrTypeDef sensor_mac;
		uint8_t sensor_id[16];
		MachineStatusTypeDef status_data;
		uint8_t idle_status;
		uint8_t open_status;
		uint8_t end_status;
		uint32_t Upworkcount;
		uint32_t dwworkcount;
		uint8_t Time[MESSAGE_TIME_BYTE];
	};
	uint8_t DataBuffer[BLE_MESSAGE_SENSOR_STATUS_Y_DATA_LEN];
} SensorStatusYTypeDef;

typedef union
{
	struct
	{
		MacAddrTypeDef sensor_mac;
		uint8_t sensor_id[3];
		uint32_t online_sales_amount;
		uint32_t coin_100_counter;
		uint32_t coin_50_counter;
		uint32_t online_test_amount;
		char TMST[14];
		char order_code[16];
	};
	uint8_t DataBuffer[BLE_MESSAGE_SENSOR_SALE_DATA_LEN];
} SensorSaleDataTypeDef;

typedef enum
{
    UART_MESSAGE_OK,
    UART_MESSAGE_INVALID_CHECKSUM,
    UART_MESSAGE_INVALID_LEN,
    UART_MESSAGE_INVALID_ROUTE,
    UART_MESSAGE_INVALID_HEAD,
    UART_MESSAGE_INVALID_FUNC
} UARTMessageVerifyTypedef;


#pragma pack (1)
/*Sensor/中继心跳协议Message部分的数据格式*/
typedef union
{
    struct
    {
        /*Sensor Mac Address*/
        MacAddrTypeDef MacAddr;
        rt_uint8_t     SensorID[MESSAGE_SLAVE_ID_BYTE];
        /*预留位*/
        rt_uint32_t    CmdID;
        rt_uint8_t     Time[MESSAGE_TIME_BYTE];
        /*电池电量*/
        rt_uint8_t     Battery;
        /*电池电压*/
        float          Voltage;
        /*信号强度*/
        rt_int8_t      RSSI;
        rt_uint8_t     device_version[23];
    };

    uint8_t DataBuff[BLE_MESSAGE_SENSOR_HEARTBEAT_LEN];
} SensorSlaveHeartbeatTypeDef;

typedef struct
{
    MacAddrTypeDef SensorMacAddr;
    uint16_t SensorRSSICount;
    int8_t SensorRSSIValue;
}SensorRSSITypeDef;

typedef struct
{
    rt_uint8_t IPAddr[WLAN_IP_ADDR_FEILD_BYTE];
    rt_uint8_t GateWay[WLAN_IP_GATEWAY_FEILD_BYTE];
    rt_uint8_t NetMask[WLAN_IP_NETMASK_FEILD_BYTE];
}StaticIPInfoTypeDef;

typedef struct
{
    rt_uint8_t DNSAddr[WLAN_DNS_ADDR_FEILD_BYTE];
}DNSInfoTypeDef;

typedef union
{
    struct
    {
        rt_uint8_t  SSIDLength;
        rt_uint8_t  SSID[WLAN_SSID_MAX_LEN_BYTE];
        rt_uint8_t  PasswordLength;
        rt_uint8_t  Password[WLAN_PASSWORD_MAX_LEN_BYTE];
        rt_uint8_t  IPType;
        StaticIPInfoTypeDef IPInfo;
        rt_uint8_t  DNSType;
        DNSInfoTypeDef DNSInfo;
    };

    rt_uint8_t DataBuffer[BLE_MESSAGE_WLAN_INFO_LEN];
}ConfigWLANInfoTypeDef;

/******Common Message *************/
typedef union 
{
    struct
    {
        rt_uint8_t State;
    };
    rt_uint8_t DataBuffer[UART_MESSAGE_ID_UNIVERSAL_RESP_LEN];
}ACK_NAK_t;

#pragma pack(pop)

extern uint8_t m_Ringbuf_full_flag;
extern int m_rec_store_id;
extern uint8_t store_id_save_flag;

extern void RecvNewMessage(rt_uint8_t *DataBuffer, rt_uint16_t DataLen);
extern MacAddrTypeDef GetMasterMac(void);
extern void SetMasterStatus(MasterStatus parameter);
#endif
