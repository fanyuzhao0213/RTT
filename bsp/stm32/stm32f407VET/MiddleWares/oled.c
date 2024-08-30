#include "oled.h"
#include "font.h"

ALIGN(RT_ALIGN_SIZE);
#define OLED_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
static rt_uint32_t MyOledThreadInit_stack = 1024;
struct rt_i2c_bus_device *oled_i2c_bus;      /* I2C总线设备句柄 */

void OLED_Init(struct rt_i2c_bus_device *bus)
{
    rt_thread_mdelay(300);
    write_reg(bus,Command_ADDR,0xAE);
	write_reg(bus,Command_ADDR,0xAE); //关闭显示
	write_reg(bus,Command_ADDR,0x00);//设置低列地址
	write_reg(bus,Command_ADDR,0x10);//设置高列地址
	write_reg(bus,Command_ADDR,0x40);//设置起始行地址,集映射RAM显示起始行(0x00~0x3F)
	write_reg(bus,Command_ADDR,0x81);//设置对比度控制寄存器
	write_reg(bus,Command_ADDR,0xCF);//设置SEG输出电流亮度
	write_reg(bus,Command_ADDR,0xA1);//段重定义设置,bit0:0,0->0;1,0->127; 0xa0左右反置 0xa1正常
	write_reg(bus,Command_ADDR,0xC8);//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数   0xc0上下反置 0xc8正常
	write_reg(bus,Command_ADDR,0xA6);//设置正常显示(设置显示方式;bit0:1,反相显示;0,正常显示	)
	write_reg(bus,Command_ADDR,0xA8);//设置驱动路数 设置多路复用比(1比64)
	write_reg(bus,Command_ADDR,0x3F);//1/64 duty(默认0X3F(1/64))
	write_reg(bus,Command_ADDR,0xD3);//设置显示偏移位移映射RAM计数器(0x00~0x3F)
	write_reg(bus,Command_ADDR,0x00);//-not offset
	write_reg(bus,Command_ADDR,0xD5);//设置显示时钟分频比/振荡器频率
	write_reg(bus,Command_ADDR,0x80);//设置分频比，设置时钟为100帧/秒
	write_reg(bus,Command_ADDR,0xD9);//设置预充电周期
	write_reg(bus,Command_ADDR,0xF1);//设置预充15个时钟，放电1个时钟([3:0],PHASE 1;[7:4],PHASE 2;)
	write_reg(bus,Command_ADDR,0xDA);//设置COM硬件引脚配置
	write_reg(bus,Command_ADDR,0x12);//[5:4]配置
	write_reg(bus,Command_ADDR,0xDB);//设置VCOMH 电压倍率
	write_reg(bus,Command_ADDR,0x40);//Set VCOM 释放电压([6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;)
	write_reg(bus,Command_ADDR,0x20);//设置页面寻址模式(0x00/0x01/0x02)
	write_reg(bus,Command_ADDR,0x02);//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	write_reg(bus,Command_ADDR,0x8D);//设置充电泵启用/禁用
	write_reg(bus,Command_ADDR,0x14);//设置(0x10禁用,0x14启用)
	write_reg(bus,Command_ADDR,0xA4);// 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) (0xa4/0xa5)
	write_reg(bus,Command_ADDR,0xA6);// 设置显示方式;bit0:1,反相显示;0,正常显示 (0xa6/a7) 
	write_reg(bus,Command_ADDR,0xAF);//开启显示	
    OLED_Clear(bus);
}

/*函数功能:开启OLED显示 */
void OLED_Display_On(struct rt_i2c_bus_device *bus)
{
    write_reg(bus,Command_ADDR,0x8D);
    write_reg(bus,Command_ADDR,0x14);
    write_reg(bus,Command_ADDR,0xAF);
}


/*
函数功能:清屏函数,清完屏,整个屏幕是黑色的!  
说明: 清屏就是向OLED里写0
      对于OLED屏,0就不显示,1才会在屏幕上显示出来一个点
*/
void OLED_Display_Off(struct rt_i2c_bus_device *bus)
{
    write_reg(bus,Command_ADDR,0x8D);
    write_reg(bus,Command_ADDR,0x10);
    write_reg(bus,Command_ADDR,0xAE);
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(struct rt_i2c_bus_device *bus,uint8_t Y, uint8_t X)
{
	write_reg(bus,Command_ADDR,0xb0 | Y);						//设置Y位置
	write_reg(bus,Command_ADDR,0x10 | ((X & 0xF0) >> 4));		//设置X位置高4位
	write_reg(bus,Command_ADDR,0x00 | (X & 0x0F));				//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(struct rt_i2c_bus_device *bus)
{
    rt_uint8_t i,n;
    for(i=0;i<8;i++)
    {
        OLED_SetCursor(bus,i,0);
        for(n=0;n<128;n++) 
			write_reg(bus,Data_ADDR,0);
    }
}


/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(struct rt_i2c_bus_device *bus, uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor(bus,(Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		write_reg(bus,Data_ADDR,OLED_F8x16[Char - ' '][i]);	//显示上半部分内容
	}
	OLED_SetCursor(bus,(Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		write_reg(bus,Data_ADDR,OLED_F8x16[Char - ' '][i + 8]);	//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(bus,Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(bus,Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(bus,Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(bus,Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(bus,Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(bus,Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(bus,Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(struct rt_i2c_bus_device *bus,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(bus,Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/* 写传感器寄存器 */
rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];
    struct rt_i2c_msg msgs;
    buf[0] = reg; //cmd
    buf[1] = data;
    msgs.addr = OLED_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 2;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

void oled_test_func(void)
{
	char buf[20] = {0};
	char buf1[20] = {0};
	char buf2[20] = "hello!everyone!";

	// 正确的格式化
	rt_sprintf(buf,"%s%d","temperature:",10);
	rt_sprintf(buf1,"%s%d","humidity:",88);

	/*OLED显示*/
	OLED_ShowChar(oled_i2c_bus,1, 1, 'A');				//1行1列显示字符A
	OLED_ShowString(oled_i2c_bus,1, 3, "HelloWorld!");	//1行3列显示字符串HelloWorld!
	OLED_ShowNum(oled_i2c_bus,2, 1, 12345, 5);			//2行1列显示十进制数字12345，长度为5
	OLED_ShowSignedNum(oled_i2c_bus,2, 7, -66, 2);		//2行7列显示有符号十进制数字-66，长度为2
	OLED_ShowHexNum(oled_i2c_bus,3, 1, 0xAA55, 4);		//3行1列显示十六进制数字0xA5A5，长度为4
	OLED_ShowBinNum(oled_i2c_bus,4, 1, 0xAA55, 16);		//4行1列显示二进制数字0xA5A5，长度为16													//C语言无法直接写出二进制数字，故需要用十六进制表示
	OLED_ShowHexNum(oled_i2c_bus,5, 1, 0xCCDD, 4);		//3行1列显示十六进制数字0xA5A5，长度为4
}
	
/*oled 线程入口函数*/
void MyOledThreadInitThread(void *parameter)
{
	rt_uint8_t pin1 = GET_PIN(C, 6);
	rt_uint8_t pin2 = GET_PIN(C, 7);
	rt_kprintf("pin1:%d,pin2:%d\n", pin1,pin2);
	
    oled_i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(OLED_I2C_BUS_NAME);  /* 查找I2C总线设备，获取I2C总线设备句柄 */
    if (oled_i2c_bus == RT_NULL)
    {
        rt_kprintf("iic sample run failed! can't find %s device!\n", OLED_I2C_BUS_NAME);
    }else{
		rt_kprintf("iic oled run successs! find %s device!\n", OLED_I2C_BUS_NAME);
	}
	
	rt_kprintf("The currrent Thread is %s!\n", "MyOledThreadInitThread");
    OLED_Init(oled_i2c_bus);
    OLED_Clear(oled_i2c_bus);
	while(1)
	{
		oled_test_func();
		rt_thread_mdelay(100);
	}
}

void OsMyOledThreadInit(rt_uint8_t ThreadPriority, rt_uint32_t ThreadTick)
{
	    /* 定义线程句柄 */
    rt_thread_t tid;
 
    /* 创建动态pin线程 ：优先级 25 ，时间片 5个系统滴答，线程栈512字节 */
    tid = rt_thread_create("oled_thread",
                  MyOledThreadInitThread,
                  RT_NULL,
                  MyOledThreadInit_stack,
                  ThreadPriority,
                  ThreadTick);
 
    /* 创建成功则启动动态线程 */
    if (tid != RT_NULL)
    {
		rt_kprintf("thread  MyOledThreadInit  start success!!\n");
    	rt_thread_startup(tid);
	}

}

