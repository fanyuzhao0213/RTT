#ifndef _DISPLAY_IIC_H_
#define _DISPLAY_IIC_H_

#include <rtthread.h>
#include <rtdbg.h>
#include <board.h>
#include <rtdevice.h>
#include "stdlib.h"

#define OLED_ADDR                  0x3C    /* 从机地址 */
#define Data_ADDR                  0x40    /* 写数据地址 */
#define Command_ADDR               0x00    /* 写命令地址 */
#define Max_Column  128

extern struct rt_i2c_bus_device *oled_i2c_bus;      		/* I2C总线设备句柄 */

rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data);
extern void OsDisplayIICThreadInit(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick);
#endif /* WORK_OLED_H_ */

