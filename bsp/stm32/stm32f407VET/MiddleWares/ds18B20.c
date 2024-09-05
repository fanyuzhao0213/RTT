/*
 * @Descripttion: ds18B20
 * @version: V1.00
 * @Author: fanyuzhao
 * @Date: 2024-08-20 12:03:45
 * @LastEditTime: 2024-08-20 11:07:39
 */

#include "ds18B20.h"
#include "board.h"
#include "rthw.h"

uint8_t ROM_ID[8];

/*功能：向DS18B20发送一个复位信号*/
void DS18B20_SendRestSingle(void)
{
	/*主机发送复位信号*/
	DS18B20_OutPut_Mode();
	
	rt_pin_write(DS18B20_PIN, 0);
	rt_hw_us_delay(750);//拉低总线480~960 us ，对 DS18B20 芯片进行复位
	rt_pin_write(DS18B20_PIN, 1);
	rt_hw_us_delay(15);         //释放总线15~60 us
}

/*
功能：检测DS18B20存在脉冲
返回值：
		0  DS18B20设备正常
		1  DS18B20设备响应复位信号失败
		2  DS18B20设备释放总线失败
*/
uint8_t DS18B20_CheckReadySingle(void)
{
	uint8_t cnt=0;
	/*1.检测存在脉冲*/
	DS18B20_InPut_Mode();
	while(DS18B20_IN && cnt < 240) //等待DS18B20 拉低总线 （60~240 us 响应复位信号）
	{
		rt_hw_us_delay(1);
		cnt++;
	}
	if(cnt>240) return 1;
	/*2.检测DS18B20是否释放总线*/	
	cnt=0;
	DS18B20_InPut_Mode();
	while((!DS18B20_IN) && cnt<240)  //判断DS18B20是否释放总线（60~240 us 响应复位信号之后会释放总线）
	{
		rt_hw_us_delay(1);
		cnt++;
	}
	if(cnt>240)	return 2;
	else return 0;
}

/*
功能：检测DS18B20设备是否正常
返回值：
			0  DS18B20设备正常
			1  DS18B20设备响应复位信号失败
			2  DS18B20设备释放总线失败
*/
uint8_t DS18B20_CheckDevice(void)
{
	DS18B20_SendRestSingle();/*1.主机发送复位信号*/
	return DS18B20_CheckReadySingle();/*2.检测存在脉冲*/
}


/*功能：向DS18B20写一个字节数据（命令）*/
uint8_t BS18B20_WriteByte(uint8_t cmd)
{
	uint8_t i=0;
	DS18B20_OutPut_Mode();
	for(i=0;i<8;i++)
	{
		rt_pin_write(DS18B20_PIN, 0);
		rt_hw_us_delay(2);     //主机拉低总线写数据时间隙2us
		rt_pin_write(DS18B20_PIN, cmd&0x01);//如果该写入位为1，必须在15us之内把总线拉高，为0 保持60us即可。
		rt_hw_us_delay(60);    //确保DS18B20已经成功读取到该位数据
		rt_pin_write(DS18B20_PIN, 1); 	//一位发送完成
		cmd >>=1;
		rt_hw_us_delay(2);     //位间隙2us
	}
	return 0;
}
 
/*功能：从DS18B20读取一个字节数据*/
uint8_t DS18B20_ReadByte(void)
{
	uint8_t i,data=0;
	for(i=0;i<8;i++)
	{
		DS18B20_OutPut_Mode();//初始化为输出模式
		rt_pin_write(DS18B20_PIN, 0);  
		rt_hw_us_delay(2);  //主机拉低总线读数据时间隙2us		
		rt_pin_write(DS18B20_PIN, 1);; //释放总线，准备读取位数据
		DS18B20_InPut_Mode(); //初始化为输入模式
		rt_hw_us_delay(10); //等待DS18B20的数据输出
		data >>=1 ;  //高位补0，默认以0为准
		if(DS18B20_IN)
		{
			data |=0x80;		
		}
		rt_hw_us_delay(60); 			//延时确保DS18B20采样周期已经过去（非常重要）
		rt_pin_write(DS18B20_PIN, 1);  //释放总线准备读取下一位位数据
	}
	return data;
}


/*
函数功能: 读取一次DS18B20的温度数据
返 回 值: 读取的温度数据
考虑的情况:  总线上只是接了一个DS18B20的情况
*/
uint16_t DS18B20_GetTemperature(void)
{
	uint16_t temp=0;
	uint8_t temp_H,temp_L;
	int intT,decT;
	
	DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
	BS18B20_WriteByte(0xCC); //跳过ROM序列检测
	BS18B20_WriteByte(0x44); //启动一次温度转换
	
	//等待温度转换完成
	while(DS18B20_ReadByte()!=0xFF){}
	
	DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
	BS18B20_WriteByte(0xCC); //跳过ROM序列检测
	BS18B20_WriteByte(0xBE); //读取温度
	
	temp_L=DS18B20_ReadByte(); //读取的温度低位数据
	temp_H=DS18B20_ReadByte(); //读取的温度高位数据
	temp=temp_L|(temp_H<<8);   //合成温度
	intT = temp>>4 ;          /*合成实际温度整数部分****精度相对上面的更高*/ 
	decT = temp&0xF ;         /*合成实际温度小数部分*/
	rt_kprintf("温度:%d.%d ℃ \n",intT,decT);
	return temp;
}

/*功能：从DS18B20读取ROM信息 (ROM_ID= 80-7f-ff-0b-10-cd-ff-ff)*/
uint8_t DS18B20_ReadRomInfo(void)
{
	uint8_t i=0;
	BS18B20_WriteByte(0x33);  /*4.启动读取ROM*/
	for(i=0;i<8;i++)
	{
		ROM_ID[i]=DS18B20_ReadByte();
	}
	rt_kprintf("ROM_ID= ");
	for(i=0;i<8;i++)
	{
		rt_kprintf("%02x-",ROM_ID[i]);
	}
	rt_kprintf("\n");
	rt_kprintf("get romid success!\n");
	return 0;
}

/*功能：匹配 DS18B20  ROM信息*/
uint8_t DS18B20_MatchROM(void)
{
	uint8_t i=0;
	BS18B20_WriteByte(0x55);  /*4.匹配64位 ROM 信息*/
	for(i=0;i<8;i++)
	{
		BS18B20_WriteByte(ROM_ID[i]);
	}
	return 0;
}

    
