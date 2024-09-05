/*
 * @Descripttion: EEPROM头文件
 */

#ifndef EEPROM_H
#define EEPROM_H

#include <rtdef.h>
#include <board.h>

//由数据手册决定
#define EEPROM_ADDR         (0x50)

#define WLAN_SSID_LEN_EEPROM_ADDR       (0x001F)
#define WLAN_SSID_LEN_FEILD_BYTE        (1)
#define WLAN_SSID_EEPROM_START_ADDR     (WLAN_SSID_LEN_EEPROM_ADDR+WLAN_SSID_LEN_FEILD_BYTE)
#define WLAN_SSID_MAX_LEN_BYTE          (32)

#define WLAN_PASSWORD_LEN_EEPROM_ADDR   (0x0040)
#define WLAN_PASSWORD_LEN_FEILD_BYTE    (1)
#define WLAN_PASSWORD_EEPROM_START_ADDR (WLAN_PASSWORD_LEN_EEPROM_ADDR+WLAN_PASSWORD_LEN_FEILD_BYTE)
#define WLAN_PASSWORD_MAX_LEN_BYTE      (64)

#define WLAN_IP_TYPE_EEPROM_ADDR        (0x0080)
#define WLAN_IP_TYPE_FEILD_BYTE         (1)
#define WLAN_IP_INFO_EEPROM_START_ADDR  (WLAN_IP_TYPE_EEPROM_ADDR+WLAN_IP_TYPE_FEILD_BYTE)
#define WLAN_IP_ADDR_FEILD_BYTE         (4)
#define WLAN_IP_GATEWAY_FEILD_BYTE      (4)
#define WLAN_IP_NETMASK_FEILD_BYTE      (4)
#define WLAN_IP_INFO_BYTE               (WLAN_IP_ADDR_FEILD_BYTE+WLAN_IP_GATEWAY_FEILD_BYTE+WLAN_IP_NETMASK_FEILD_BYTE)

#define WLAN_DNS_TYPE_EEPROM_ADDR        (0x0090)
#define WLAN_DNS_TYPE_FEILD_BYTE         (1)
#define WLAN_DNS_INFO_EEPROM_START_ADDR  (WLAN_DNS_TYPE_EEPROM_ADDR+WLAN_DNS_TYPE_FEILD_BYTE)
#define WLAN_DNS_ADDR_FEILD_BYTE         (4)
#define WLAN_DNS_INFO_BYTE               (WLAN_DNS_ADDR_FEILD_BYTE)

#define WLAN_RESET_MCU_EEPROM_ADDR       (0x00A0)
#define WLAN_RESET_MCU_FEILD_BYTE        (1)


#define WLAN_STORE_ID_EEPROM_ADDR        (0x00B0)
// 网络地址举例："255.255.255.255", 最大为15个字节，+1预留字符串结束符
#define WLAN_NET_ADDR_STRING_BYTE        (15+1)


#define TEST_EEPROM_ADDR        (0x00C0)

#define UP_BLE_MAC_EEPROM_ADDR 			(0x00D0)
#define UP_BLE_MAC_EEPROM_LEN 			(6)

typedef enum
{
    NORMAL_RESET_MCU = 0,
    WLAN_RESET_MCU,
}ResetMCUTypeDef;

#pragma pack (1)
typedef enum
{
    WLAN_DYNAMIC_IP = 0x00,
    WLAN_STATIC_IP,
    WLAN_IP_TYPE_MAX
}WLANIPTypeDef;

typedef union
{
    struct
    {
        rt_uint8_t IPAddr[WLAN_IP_ADDR_FEILD_BYTE];
        rt_uint8_t IPGateWay[WLAN_IP_GATEWAY_FEILD_BYTE];
        rt_uint8_t IPNetMask[WLAN_IP_NETMASK_FEILD_BYTE];
    }IPInfo;

    rt_uint8_t DataBuff[WLAN_IP_INFO_BYTE];

}WLANStaticIPInfoTypeDef;

typedef struct
{
    rt_uint8_t IPAddr[WLAN_NET_ADDR_STRING_BYTE];
    rt_uint8_t IPGateWay[WLAN_NET_ADDR_STRING_BYTE];
    rt_uint8_t IPNetMask[WLAN_NET_ADDR_STRING_BYTE];
}WLANStaticIPInfoStringTypeDef;

typedef enum
{
    WLAN_DYNAMIC_DNS = 0x00,
    WLAN_STATIC_DNS,
    WLAN_DNS_TYPE_MAX
}WLANDNSTypeDef;

typedef union
{
    struct
    {
        rt_uint8_t DNSAddr[WLAN_DNS_ADDR_FEILD_BYTE];
    }DNSInfo;

    rt_uint8_t DataBuff[WLAN_DNS_INFO_BYTE];

}WLANStaticDNSInfoTypeDef;

typedef struct
{
    rt_uint8_t DNSAddr[WLAN_NET_ADDR_STRING_BYTE];
}WLANStaticDNSInfoStringTypeDef;

#pragma pack ()

extern rt_uint8_t EepromHwInit(void);
extern rt_err_t ReadEeprom(rt_uint16_t ReadAddr, rt_uint8_t *buf, rt_uint16_t len);
extern rt_err_t WriteEeprom(rt_uint16_t WriteAddr, rt_uint8_t *data, rt_uint16_t len);

extern void SetWLANSSIDLength(rt_uint8_t SetValue);
extern rt_uint8_t GetWLANSSIDLength(void);
extern void SetWLANPasswordLength(rt_uint8_t SetValue);
extern rt_uint8_t GetWLANPasswordLength(void);

extern rt_uint8_t *GetWLANSSID(void);
extern rt_uint8_t *GetWLANPassword(void);

extern rt_err_t WriteWLANSSIDLenToEEPROM(rt_uint8_t SSIDLength);
extern rt_err_t ReadWLANSSIDLenFromEEPROM(void);
extern rt_err_t WriteWLANSSIDToEEPROM(rt_uint8_t *pSSIDData);
extern rt_err_t ReadWLANSSIDFromEEPROM(void);

extern rt_err_t WriteWLANPasswordLenToEEPROM(rt_uint8_t PasswordLength);
extern rt_err_t ReadWLANPasswordLenFromEEPROM(void);
extern rt_err_t WriteWLANPasswordToEEPROM(rt_uint8_t *pPasswordData);
extern rt_err_t ReadWLANPasswordFromEEPROM(void);

extern void SetWLANIPType(WLANIPTypeDef SetValue);
extern WLANIPTypeDef GetWLANIPType(void);
extern rt_err_t WriteWLANIPTypeToEEPROM(WLANIPTypeDef IPType);
extern rt_err_t ReadWLANIPTypeFromEEPROM(void);
extern void SetWLANStaticIPInfo(WLANStaticIPInfoTypeDef SetValue);
extern WLANStaticIPInfoTypeDef GetWLANStaticIPInfo(void);
extern rt_err_t WriteWLANIPInfoToEEPROM(uint8_t *IPinfo);
extern rt_err_t ReadWLANIPInfoFromEEPROM(void);

extern void SetWLANDNSType(WLANDNSTypeDef SetValue);
extern WLANDNSTypeDef GetWLANDNSType(void);
extern rt_err_t WriteWLANDNSTypeToEEPROM(WLANDNSTypeDef DNSType);
extern rt_err_t ReadWLANDNSTypeFromEEPROM(void);
extern void SetWLANStaticDNSInfo(WLANStaticDNSInfoTypeDef SetValue);
extern WLANStaticDNSInfoTypeDef GetWLANStaticDNSInfo(void);
extern rt_err_t WriteWLANDNSInfoToEEPROM(uint8_t *DNSInfo);
extern rt_err_t ReadWLANDNSInfoFromEEPROM(void);

extern void GetIPInfoString(WLANStaticIPInfoStringTypeDef *pIPInfoString);
extern void GetDNSInfoString(WLANStaticDNSInfoStringTypeDef *pDNSInfoString);

extern void LoadWLANInfoFromEEPROM(void);
extern void SetWLANResetMCUFlag(ResetMCUTypeDef SetValue);
extern ResetMCUTypeDef GetWLANResetMCUFlag(void);
extern rt_err_t WriteWLANResetMCUFlagToEEPROM(ResetMCUTypeDef *pFlagValue);
extern rt_err_t ReadWLANResetMCUFlagFromEEPROM(void);
extern void LoadWLANInfoFromEEPROM(void);
#endif
