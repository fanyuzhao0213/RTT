/*
 * @Descripttion: EEPROM头文件源文件
 */
#include "EEPROM.h"



#define EEPROM_I2C_BUS_NAME "i2c2"
struct rt_i2c_bus_device *EepromI2CBus;

/**
 * @name: EepromHwInit
 * @msg:  EEPROM初始化
 * @param [in]
 *         无
 * @param [out]
 *         无 
 * @return 1:初始化失败；0：初始化成功
 * @note:  无
 *         
 */
rt_uint8_t EepromHwInit(void)
{
    EepromI2CBus = (struct rt_i2c_bus_device *)rt_device_find(EEPROM_I2C_BUS_NAME);

    if (RT_NULL == EepromI2CBus)
    {
        rt_kprintf("can't find EEPROM %s device!\n", EEPROM_I2C_BUS_NAME);
        return RT_ERROR;
    }
    else
    {
		rt_kprintf("find EEPROM %s device!\n", EEPROM_I2C_BUS_NAME);
        return RT_EOK;
    } 
}
// INIT_DEVICE_EXPORT(EepromHwInit);

/**
 * @name: ReadEeprom
 * @msg:  读EEPROM
 * @param [in]
 *         ReadAddr：EEPROM读的开始地址
 *         len：读取的数据长度
 * @param [out]
 *         buf：读数据缓冲区指针
 * @return -1:读失败；0：读取成功
 * @note:  1)读数据前需要首先发送读的地址
 *         2)EEPROM的的寄存器地址为双字节,首先发送寄存器地址的高8位字节地址,然后发送寄存器地址的低8位字节地址
 *         
 */
rt_err_t ReadEeprom(rt_uint16_t ReadAddr, rt_uint8_t *buf, rt_uint16_t len)
{
    struct rt_i2c_msg msgs[2] = {0};
    rt_uint8_t TempBuf[2] = {0};

    TempBuf[0] = (ReadAddr >> 8);
    TempBuf[1] = ReadAddr;

    //写读取的地址
    msgs[0].addr  = EEPROM_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = TempBuf;
    msgs[0].len   = 2;

    msgs[1].addr  = EEPROM_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = len;

    if (2 == rt_i2c_transfer(EepromI2CBus, msgs, 2))
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to read EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: WriteEeprom
 * @msg:  写EEPROM
 * @param [in]
 *         WriteAddr：EEPROM开始写的地址
 *         data：写数据缓冲区指针
 *         len： 写数据长度
 * @param [out]
 *         无
 * @return -1:读失败；0：读取成功
 * @note:  1)写数据前需要首先发送写的地址
 *         2)EEPROM的的寄存器地址为双字节,首先发送寄存器地址的高8位字节地址,然后发送寄存器地址的低8位字节地址
 *         
 */
rt_err_t WriteEeprom(rt_uint16_t WriteAddr, rt_uint8_t *data, rt_uint16_t len)
{
    struct rt_i2c_msg msgs = {0};
    rt_uint8_t buf[len+2];

    //写地址
    buf[0] = (WriteAddr >> 8);
    buf[1] = WriteAddr;
    rt_memcpy(buf+2, data, len);

    msgs.addr  = EEPROM_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buf;
    msgs.len   = (len + 2);

    if (1 == rt_i2c_transfer(EepromI2CBus, &msgs, 1))
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to write EEPROM!\n");
        return -RT_ERROR;
    }
}

void eepromTest(void)
{
	uint8_t test_buffer[10] = {0xAA,0xBB,0xCC,0x4,0x05,0x06,0x08,0x09,0x07,0x10};
	uint8_t read_buffer[10] = {0};
	
    if (RT_EOK == WriteEeprom(TEST_EEPROM_ADDR, test_buffer, 10))
    {
        rt_kprintf("test write EEPROM success!\n");
    }else
	{
		rt_kprintf("test write EEPROM Failed!\n");
	}

    if (RT_EOK == ReadEeprom(TEST_EEPROM_ADDR, read_buffer, 10))
    {
		rt_kprintf("test read EEPROM success!\n");
		LOG_HEXDUMP(read_buffer,sizeof(test_buffer),"read eeprom");
    }else
	{
		rt_kprintf("test read EEPROM Failed!\n");
	}
}

MSH_CMD_EXPORT(eepromTest, eeprom Test);
/***************************WLAN参数定义*******************************/
rt_uint8_t gWLANSSIDLength = 0x00;
rt_uint8_t gWLANPasswordLength = 0x00;
// 给字符串结束符预留1字节
rt_uint8_t aWLANSSID[WLAN_SSID_MAX_LEN_BYTE+1] = {0x00};
// 给字符串结束符预留1字节
rt_uint8_t aWLANPassword[WLAN_PASSWORD_MAX_LEN_BYTE+1] = {0x00};
/**
 * @name: GetWLANSSIDLength
 * @msg:  获得WLAN SSID 长度
 * @param [in]
 *         SetValue: 设置值
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SetWLANSSIDLength(rt_uint8_t SetValue)
{
    gWLANSSIDLength = SetValue;

    return ;
}

/**
 * @name: GetWLANSSIDLength
 * @msg:  获得WLAN SSID 长度
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return WLAN_SSID 长度
 * @note:  无
 */
rt_uint8_t GetWLANSSIDLength(void)
{
    return gWLANSSIDLength;
}

/**
 * @name: SetWLANPasswordLength
 * @msg:  设置WLAN密码长度
 * @param [in]
 *         SetValue: 设置值
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void SetWLANPasswordLength(rt_uint8_t SetValue)
{
    gWLANPasswordLength = SetValue;

    return ;
}

/**
 * @name: GetWLANPasswordLength
 * @msg:  获得WLAN密码长度
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return WLAN_password 长度
 * @note:  无
 */
rt_uint8_t GetWLANPasswordLength(void)
{
    return gWLANPasswordLength;
}


/**
 * @name: GetWLANSSID
 * @msg:  返回WLANSSID字符串
 * @param  无
 * @return WLANSSID字符串指针
 * @note:  WLANSSID字符串长度需要根据
 */
rt_uint8_t *GetWLANSSID(void)
{
    return aWLANSSID;
}

/**
 * @name: GetWLANPassword
 * @msg:  返回WLANPassword字符串
 * @param  无
 * @return WLANPassword字符串指针
 * @note:  WLANPassword字符串长度需要根据
 */
rt_uint8_t *GetWLANPassword(void)
{
    return aWLANPassword;
}

/***********************************WLAN_SSID_Length*************************************/
/**
 * @name: WriteWLANSSIDLenToEEPROM
 * @msg:  将WLAN SSID 长度写入EEPROM
 * @param [in]
 *         SSIDLength: WLAN SSID
 * @param [out]
 *         无
 * @return -1:写入失败；0:写入成功
 * @note:  WiFi协议规定SSID 最大长度为32字节
 */
rt_err_t WriteWLANSSIDLenToEEPROM(rt_uint8_t SSIDLength)
{
    if (SSIDLength > WLAN_SSID_MAX_LEN_BYTE)
    {
        rt_kprintf("the SSID length exceeds the maximum value!\n");
        return -RT_ERROR;
    }

    if (RT_EOK == WriteEeprom(WLAN_SSID_LEN_EEPROM_ADDR, &SSIDLength, WLAN_SSID_LEN_FEILD_BYTE))
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to write WLAN_SSID_Length to EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: ReadWLANSSIDLenFromEEPROM
 * @msg:  从EEPROM中读取WLAN SSID长度字段
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return -1:读取失败；0:读取成功; 4:EEPROM无有效数据
 * @note:  无
 */
rt_err_t ReadWLANSSIDLenFromEEPROM(void)
{
    rt_uint8_t TempVar = 0x00;

    if (RT_EOK == ReadEeprom(WLAN_SSID_LEN_EEPROM_ADDR, &TempVar, WLAN_SSID_LEN_FEILD_BYTE))
    {
        if (0xFF == TempVar)   //EEPROM指定区域无有效数据
        {
            rt_kprintf("No WLAN_SSID_LEN in EEPROM and use default value!\n");
            //加载并保存默认的数据到EEPROM.
            WriteWLANSSIDLenToEEPROM((rt_uint8_t)rt_strlen(ESP8266_SAMPLE_WIFI_SSID));
            SetWLANSSIDLength((rt_uint8_t)rt_strlen(ESP8266_SAMPLE_WIFI_SSID));
            return RT_EEMPTY;
        }
        else
        {
            SetWLANSSIDLength(TempVar);
            return RT_EOK;
        }
    }
    else
    {
        rt_kprintf("Failed to get WLANSSID length from EEPROM!\n");
        return -RT_ERROR;
    }
}

/***********************************WLAN_SSID*************************************/
/**
 * @name: WriteWLANSSIDToEEPROM
 * @msg:  将WLAN SSID写入EEPROM
 * @param [in]
 *         pSSIDData: WLAN SSID数据
 * @param [out]
 *         无
 * @return -1:写入失败；0:写入成功
 * @note:  WiFi协议规定SSID 最大长度为32字节
 */
rt_err_t WriteWLANSSIDToEEPROM(rt_uint8_t *pSSIDData)
{
    if (RT_EOK == WriteEeprom(WLAN_SSID_EEPROM_START_ADDR, pSSIDData, GetWLANSSIDLength()))
    {
		rt_kprintf("wifi name is %s\n", pSSIDData);
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to write WLAN_SSID to EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: ReadWLANSSIDFromEEPROM
 * @msg:  从EEPROM中读取WLAN SSID
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return -1:读取失败；0:读取成功; 4:EEPROM无有效数据
 * @note:  无
 */
rt_err_t ReadWLANSSIDFromEEPROM(void)
{
    rt_uint8_t TempBuf[WLAN_SSID_MAX_LEN_BYTE+1] = {0};
    rt_uint8_t i = 0;
    rt_uint8_t SSIDLength = GetWLANSSIDLength();

    if (RT_EOK == ReadEeprom(WLAN_SSID_EEPROM_START_ADDR, TempBuf, SSIDLength))
    {
        //判断指定的EEPROM是否为空白区
        for (i = 0; i < SSIDLength; i++)
        {
			rt_kprintf("From EEPROM SSID is %02x\n",TempBuf);
            if (0xFF != TempBuf[i])
            {
                break;
            }
        }

        if (SSIDLength == i)   //EEPROM指定区域无有效数据
        {
            rt_kprintf("No WLAN_SSID in EEPROM and use default value!\n");
            //加载并保存默认的数据到EEPROM.
            WriteWLANSSIDToEEPROM((rt_uint8_t *)ESP8266_SAMPLE_WIFI_SSID);
            SetWLANSSIDLength(rt_strlen(ESP8266_SAMPLE_WIFI_SSID));
            return RT_EEMPTY;
        }
        else
        {
            TempBuf[SSIDLength] = '\0';
            rt_memset(aWLANSSID, 0, sizeof(aWLANSSID));
            // 写入字符结束符
            rt_memcpy(aWLANSSID, TempBuf, SSIDLength+1);
            return RT_EOK;
        }
    }
    else
    {
        rt_kprintf("Failed to get WLAN_SSID from EEPROM!\n");
        return -RT_ERROR;
    }
}

/***********************************WLAN_Password_Length*************************************/
/**
 * @name: WriteWLANPasswordLenToEEPROM
 * @msg:  将WLAN password 长度写入EEPROM
 * @param [in]
 *         PasswordLength: WLAN PASSWORD
 * @param [out]
 *         无
 * @return -1:写入失败；0:写入成功
 * @note:  ESP8266规定PASSWORD 最大长度为64字节
 */
rt_err_t WriteWLANPasswordLenToEEPROM(rt_uint8_t PasswordLength)
{
    if (PasswordLength > WLAN_PASSWORD_MAX_LEN_BYTE)
    {
        rt_kprintf("the PASSWORD length exceeds the maximum value!\n");
        return -RT_ERROR;
    }

    if (RT_EOK == WriteEeprom(WLAN_PASSWORD_LEN_EEPROM_ADDR, &PasswordLength, WLAN_PASSWORD_LEN_FEILD_BYTE))
    {
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to write WLAN_PASSWORD_Length to EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: ReadWLANPasswordLenFromEEPROM
 * @msg:  从EEPROM中读取WLAN PASSWORD长度字段
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return -1:读取失败；0:读取成功; 4:EEPROM无有效数据
 * @note:  无
 */
rt_err_t ReadWLANPasswordLenFromEEPROM(void)
{
    rt_uint8_t TempVar = 0x00;

    if (RT_EOK == ReadEeprom(WLAN_PASSWORD_LEN_EEPROM_ADDR, &TempVar, WLAN_PASSWORD_LEN_FEILD_BYTE))
    {
        if (0xFF == TempVar)   //EEPROM指定区域无有效数据
        {
            rt_kprintf("No WLAN_PASSWORD_LEN in EEPROM and use default value!\n");
            //加载并保存默认的数据到EEPROM.
            WriteWLANPasswordLenToEEPROM((rt_uint8_t)rt_strlen(ESP8266_SAMPLE_WIFI_PASSWORD));
            SetWLANPasswordLength((rt_uint8_t)rt_strlen(ESP8266_SAMPLE_WIFI_PASSWORD));
            return RT_EEMPTY;
        }
        else
        {
            SetWLANPasswordLength(TempVar);
            return RT_EOK;
        }
    }
    else
    {
        rt_kprintf("Failed to get WLANPASSWORD length from EEPROM!\n");
        return -RT_ERROR;
    }
}

/***********************************WLAN_Password*************************************/
/**
 * @name: WriteWLANPasswordToEEPROM
 * @msg:  将WLAN password写入EEPROM
 * @param [in]
 *         pPasswordData: WLAN password数据
 * @param [out]
 *         无
 * @return -1:写入失败；0:写入成功
 * @note:  ESP8266规定password 最大长度为64字节
 */
rt_err_t WriteWLANPasswordToEEPROM(rt_uint8_t *pPasswordData)
{
    if (RT_EOK == WriteEeprom(WLAN_PASSWORD_EEPROM_START_ADDR, pPasswordData, GetWLANPasswordLength()))
    {
		rt_kprintf("wifi password is %s\n", pPasswordData);
        return RT_EOK;
    }
    else
    {
        rt_kprintf("Failed to write WLAN_PASSWORD to EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: ReadWLANPasswordFromEEPROM
 * @msg:  从EEPROM中读取WLAN password
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return -1:读取失败；0:读取成功; 4:EEPROM无有效数据
 * @note:  无
 */
rt_err_t ReadWLANPasswordFromEEPROM(void)
{
    rt_uint8_t TempBuf[WLAN_PASSWORD_MAX_LEN_BYTE+1] = {0};
    rt_uint8_t i = 0;
    rt_uint8_t PasswordLength = GetWLANPasswordLength();

    if (RT_EOK == ReadEeprom(WLAN_PASSWORD_EEPROM_START_ADDR, TempBuf, PasswordLength))
    {
        //判断指定的EEPROM是否为空白区
        for (i = 0; i < PasswordLength; i++)
        {
			rt_kprintf("From EEPROM Password is %S\n",TempBuf);
            if (0xFF != TempBuf[i])
            {
                break;
            }
        } 

        if (PasswordLength == i)   //EEPROM指定区域无有效数据
        {
            rt_kprintf("No WLAN_PASSWORD in EEPROM and use default value!\n");
            //加载并保存默认的数据到EEPROM.
            WriteWLANPasswordToEEPROM((rt_uint8_t *)ESP8266_SAMPLE_WIFI_PASSWORD);
            SetWLANPasswordLength(rt_strlen(ESP8266_SAMPLE_WIFI_PASSWORD));
            return RT_EEMPTY;
        }
        else
        {
            TempBuf[PasswordLength] = '\0';
            rt_memset(aWLANPassword, 0, sizeof(aWLANPassword));
            // 将字符结束符写入
            rt_memcpy(aWLANPassword, TempBuf, PasswordLength+1);
            return RT_EOK;
        }
    }
    else
    {
        rt_kprintf("Failed to get WLAN_PASSWORD from EEPROM!\n");
        return -RT_ERROR;
    }
}

/**
 * @name: LoadWLANInfoFromEEPROM
 * @msg:  从EEPROM中加载WLAN参数
 * @param [in]
 *         无
 * @param [out]
 *         无
 * @return 无
 * @note:  无
 */
void LoadWLANInfoFromEEPROM(void)
{
    // WLAN_SSID to aWLANSSID
    ReadWLANSSIDLenFromEEPROM();
    ReadWLANSSIDFromEEPROM();

    // WLAN_PWD to aWLANPassword
    ReadWLANPasswordLenFromEEPROM();
    ReadWLANPasswordFromEEPROM();
}