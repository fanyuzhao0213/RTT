#ifndef _OLED_H_
#define _OLED_H_

#include <rtthread.h>
#include <rtdbg.h>
#include <board.h>
#include <rtdevice.h>
#include "stdlib.h"

#define OLED_ADDR                  0x3C    /* 从机地址 */
#define Data_ADDR                  0x40    /* 写数据地址 */
#define Command_ADDR               0x00    /* 写命令地址 */
#define Max_Column  128

void OLED_Init(struct rt_i2c_bus_device *bus);
void OLED_Clear(struct rt_i2c_bus_device *bus);
void OLED_ShowChar(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data);

extern void OsMyOledThreadInit(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick);
#endif /* WORK_OLED_H_ */

