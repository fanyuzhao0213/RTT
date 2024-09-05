/*
 * @Descripttion: __DS18B20
 */

#ifndef __DS18B20_H
#define __DS18B20_H

#include <rtthread.h>
 
#define DS18B20_PIN    GET_PIN(D, 6)

#define DS18B20_InPut_Mode()  	rt_pin_mode(DS18B20_PIN, PIN_MODE_INPUT);
#define DS18B20_OutPut_Mode() 	rt_pin_mode(DS18B20_PIN, PIN_MODE_OUTPUT);

#define DS18B20_IN  			rt_pin_read(DS18B20_PIN)

extern uint8_t DS18B20_ReadRomInfo(void);
extern uint8_t DS18B20_CheckDevice(void);
extern uint16_t DS18B20_GetTemperature(void);
/* 读取 DS18B20 的温度 */
extern float ds18b20_read_temperature(void);
#endif
