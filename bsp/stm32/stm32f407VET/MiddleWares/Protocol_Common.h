/*
 * @Descripttion: 亲机蓝牙与MCU通讯和MCU与服务器通讯协议的共有协议
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <rtthread.h>


/**********调试开关**********/
#define DEBUG_MODE

/**********亲机蓝牙和亲机MCU通讯报文各字段字节数**********/
#define    MESSAGE_HEAD_BYTE                (2)
#define    MESSAGE_LENGTH_BYTE              (2)
#define    MESSAGE_INSTRUCTION_ID_BYTE      (2)
#define    MESSAGE_ROUTE_COUNTER_BYTE       (1)
#define    MESSAGE_MAC_BYTE                 (6)
#define    MESSAGE_ROUTE_MAX_VALUE          (4)
#define    MESSAGE_ROUTE_MAX_BYTE           (MESSAGE_ROUTE_MAX_VALUE * MESSAGE_MAC_BYTE)
#define    MESSAGE_SCANNED_SENSOR_COUNTER_BYTE (2)
#define    MESSAGE_MASTER_MAC_BYTE          MESSAGE_MAC_BYTE
#define    MESSAGE_SENSOR_MAC_BYTE          MESSAGE_MAC_BYTE
#define    MESSAGE_SENSOR_RSSI_COUNT_BYTE   (2)
#define    MESSAGE_SENSOR_RSSI_BYTE         (1)
#define    MESSAGE_SENSOR_ID_BYTE           (16)
#define    MESSAGE_SLAVE_MAC_BYTE           (6)
#define    MESSAGE_SLAVE_ID_BYTE            (16)
#define    MESSAGE_FAULT_DEV_MAC_BYTE       (6)
#define    MESSAGE_CMD_ID_BYTE              (4)
#define    MESSAGE_TIME_BYTE                (14)

/***************各字段偏移位置******************/
//每包数据包头为2字节
#define    MESSAGE_HEAD1_OFFSET             (0)
#define    MESSAGE_HEAD2_OFFSET             (1)
// 字节长度为2字节
#define    MESSAGE_LENGTH_OFFSET            (2)
// 指令ID为2字节
#define    MESSAGE_INSTRUCTION_ID_OFFSET    (4)
#define    MESSAGE_ROUTE_COUNTER_OFFSET     (6)
// 【Note】由于Message部分字段及次序不一样，因此不再描述

//扫描到sensor总数-低字节（仅RSSI协议，其亲机MAC偏移也有所变化）
#define    MESSAGE_RSSI_SCANNED_SENSOR_COUNTER_L_OFFSET (7)
//扫描到sensor总数-高字节（仅RSSI协议，其亲机MAC偏移也有所变化）
#define    MESSAGE_RSSI_SCANNED_SENSOR_COUNTER_H_OFFSET (8)

/**********通讯报文协议包头**********/
// 上行协议和上行协议的应答协议包头相同；下行协议和下行协议的应答协议包头相同
#define   UART_HEADER_ID                    	(0xAA)
#define   UP_MASTER_SMALL_ID               		(0xBB)

/**********通用协议ID**********/
#define   UART_MESSAGE_ID_INVALID              (0xFFFF)
#define   UART_MESSAGE_ID_INDEX_INVALID        (0xFF)
#define   UART_MESSAGE_ID_UNIVERSAL_RESP_LEN   (1)
// 协议最小长度，(MESSAGE_HEAD_BYTE+MESSAGE_LENGTH_BYTE+MESSAGE_INSTRUCTION_ID_BYTE+MESSAGE_ROUTE_COUNTER_BYTE+MESSAGE_CHECK_BYTE)
#define   UART_MESSAGE_MIN_LEN_BYTE            (8)

#define   SCANNED_SENSOR_MAX_COUNTER          (150)
// 根据ESP8266 CIPSEND最大数据传输长度2048计算知，该值最大不能超过40
#define   RSSI_SINGLE_PACKET_MAX_COUNTER      (40)

/**********上行协议ID*********/
typedef enum
{
	BLE_MESSAGE_ID_SENSOR_STATUS_Y_DATA = 0x0703,
	BLE_MESSAGE_ID_SMALL_SALE_DATA = 0x00B5,
} UplinkMessageIDTypeDef;

/*20240325add*/
#define BLE_MESSAGE_SENSOR_STATUS_Y_DATA_LEN (135)
#define BLE_MESSAGE_SENSOR_SALE_DATA_LEN (55)

//需要根据协议计算对应的值
//协议中Message部分的内容，包括：sensor_mac, sensor_id等
// TODO: lxl 后续替换为宏定义
#define BLE_MESSAGE_SENSOR_HEARTBEAT_LEN           (69)
//该长度为协议负载数据的最大长度
#define BLE_MESSAGE_UPLOAD_RSSI_LEN                (9*SCANNED_SENSOR_MAX_COUNTER)
#define BLE_MESSAGE_MASTER_BLE_HEARTBEAT_LEN       (20)
#define BLE_MESSAGE_SALE_LEN                       (107)
#define BLE_MESSAGE_MASTER_MCU_REQ_SYNC_TIME_LEN   (20)
#define BLE_MESSAGE_KEY_LEN                        (264)
#define BLE_MESSAGE_GET_WLAN_INFO_LEN              (0)
//该长度为协议负载数据的最大长度
#define BLE_MESSAGE_WLAN_INFO_LEN                  (116)
//下行指令的响应
#define BLE_MESSAGE_RESET_MACHINE_RESP_LEN         (33)
#define BLE_MESSAGE_RESET_KEY_RESP_LEN             (271)
#define BLE_MESSAGE_SLEEP_TIME_RESP_LEN            (33)
#define BLE_MESSAGE_RESET_MASTER_BLE_RESP_LEN      (7)
#define BLE_MESSAGE_CTRL_MASTER_BLE_ADV_RESP_LEN   (7)
//长度必须要大于上述响应数据包长度
#define BLE_MESSAGE_MAX_DATA_LEN                   (1400)

/**********下行协议ID*********/
typedef enum
{
    BLE_MESSAGE_ID_MASTER_BLE_HEARTBEAT_RESP      = 0x0102,
    BLE_MESSAGE_ID_MASTER_MCU_REQ_SYNC_TIME_RESP  = 0x0104,
    // 本质是和BLE_MESSAGE_ID_GET_WLAN_INFO相同，仅解决作为上下行ID类型定义名冲突
    BLE_MESSAGE_ID_SEND_WLAN_INFO                 = 0x0210,
    BLE_MESSAGE_ID_RESET_MACHINE                  = 0x0300,
    BLE_MESSAGE_ID_RESET_KEY                      = 0x0301,
    BLE_MESSAGE_ID_RESET_MASTER_BLE               = 0x0302,
    BLE_MESSAGE_ID_CTRL_MASTER_BLE_ADV            = 0x0303,
    BLE_MESSAGE_ID_UPDATE_FIRMWARE                = 0x0304,
    BLE_MESSAGE_ID_CONFIG_WLAN_RESULT             = 0x0310,
    BLE_MESSAGE_ID_SET_SLEEP_TIME                 = 0x0400,
    DOWN_MESSAGE_NUM                              = 10,
}DownlinkMessageIDTypeDef;

//需要根据协议计算对应的值
//协议中Message部分的内容，包括：sensor_mac, sensor_id等
#define BLE_MESSAGE_MASTER_BLE_HEARTBEAT_RESP_LEN  (21)
#define BLE_MESSAGE_RESET_MACHINE_LEN              (10)
#define BLE_MESSAGE_RESET_KEY_LEN                  (10)
#define BLE_MESSAGE_SLEEP_TIME_LEN                 (22)
#define BLE_MESSAGE_RESET_MASTER_BLE_LEN           (6)
#define BLE_MESSAGE_CTRL_MASTER_BLE_ADV_LEN        (7)
// 按照WLAN_SSID和Password最大值计算
#define BLE_MESSAGE_WLAN_INFO_LEN                  (116)
#define BLE_MESSAGE_CONFIG_WLAN_RESULT_LEN         (1)


enum UniversalResponseResult
{
    RECV_SUCCESS = 0x00,
    CHECKSUM_ERROR,
    INVALIDE_MASSAGE_ID,
};

enum SystemFaultCode
{
    STOP_MASTER_BLE_ADV,
    RESTART_MASTER_BLE_ADV,
    RESET_MASTER_BLE,
    RESTART_MCU,
    UPDATE_FIRMWARE,
    RESTART_WLAN_MODULE,
    SERVER_FAILURE,
    WLAN_DEVICE_ABNORMAL,
	WLAN_ROUTER_ABNORMAL,
	WLAN_SERVER_ABNORMAL,
	LAN_ABNORMAL,
};

typedef enum
{
    MASTER_MCU_OK    = (1<<0),
    MASTER_BLE_FAULT = (1<<1),
}MasterStatus;

typedef enum
{
    ALLOW_RESET = 0,
    DISALLOW_RESET,
}ResetType;

#endif



