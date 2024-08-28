/*
 * @Descripttion: CRC校验源文件
 */

#include "VerificationAlgorithm.h"
#include "stdint.h"
#include "string.h"
#include <rtthread.h>
#include <rtdevice.h>

/**
 * @name: CRC16_CCITT
 * @msg:  该函数可生成16位的CCITT类型CRC校验位，Polynomial: 0x1021，Initial Value: 0x0000
 * @param [in]
 *         uint8_t *puchMsg：待生成校验位的原始数据
 *         uint16_t usDataLen： puchMsg数据长度
 * @param [out]
 *         无
 * @return 返回值位生成的校验位
 * @note:  无
 */
uint16_t CRC16_CCITT(const uint8_t *puchMsg, uint16_t usDataLen)
{
    uint16_t wCRCin = 0x0000;
    uint16_t wCPoly = 0x1021;
    uint8_t wChar = 0;
    uint8_t i = 0;
    while (usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);

        for (i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }
    // InvertUint16(&wCRCin, &wCRCin);
    return (wCRCin);
}

/**
 * @name: CRC16_CCITT_Check
 * @msg:  校验16位的CCITT是否正确
 * @param [in]
 *         uint8_t *puchMsg：待生成校验位的原始数据
 *         uint16_t usDataLen： puchMsg数据长度
 * @param [out]
 *         无
 * @return 1:校验失败；0：校验成功
 * @note:  无
 */
uint8_t CRC16_CCITT_Check(const uint8_t *puchMsg, uint16_t usDataLen)
{
    uint16_t crc = 0;
	
    crc = CRC16_CCITT(puchMsg, usDataLen-2);
	rt_kprintf("[CRC] crc value:%04x",crc);
	
    if(memcmp(&crc,&puchMsg[usDataLen-2],2) != 0)
        return 0;
    else
        return 1;
}
