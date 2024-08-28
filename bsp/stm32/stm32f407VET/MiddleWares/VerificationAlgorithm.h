/*
 * @Descripttion: CRC校验头文件
 */

#ifndef VERIFICATION_ALGORITHM_H
#define VERIFICATION_ALGORITHM_H

#include "stdint.h"

#define CRC_RESULT_DEFAULT (0xFFFF)

extern uint16_t CRC16_CCITT(const uint8_t *puchMsg, uint16_t usDataLen);
extern uint8_t CRC16_CCITT_Check(const uint8_t *puchMsg, uint16_t usDataLen);

#endif
