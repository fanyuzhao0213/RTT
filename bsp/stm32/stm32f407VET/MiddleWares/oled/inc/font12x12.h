/**
  **********************************************************************************************************************
  * @file    font12x12.h
  * @brief   该文件包含长宽比为 12 * 12 汉字, 6 * 12 ASCII字符 字库：
  *          ASCII字符集: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
  *           + 取模方式设置：
  *             ++ 取模软件：PCtolLCD2002-(字符模式)
  *             ++ 取模设置：阴码 + 列行式 + 逆向 + 16进制 + C51格式
  *             ++ 建议设置：C51格式选择自定义格式：去掉行前缀 "{" 和 行后缀 "}", 行后缀保留 ","
  *
  **********************************************************************************************************************
  * @attention
  * 需保证文件的编码格式为 GB2312
  *
  **********************************************************************************************************************
  */

/**
  * @brief      12 * 12 汉字字符集点阵索引
  * @note       分别对应点阵数据 FONT_12X12_DATA
  */
const unsigned char FONT_12X12_IDX[] =	
{
"大橙子疯西安途丁"
};

/**
  * @brief      12 * 12 汉字字符集点阵数据.
  */
const unsigned char FONT_12X12_DATA[] =	
{
0x10,0x10,0x10,0x10,0xD0,0x3F,0xD0,0x10,0x10,0x10,0x10,0x00,0x08,0x08,0x04,0x03,0x00,0x00,0x00,0x03,0x04,0x08,0x08,0x00,/*"大",0*/
0x88,0x68,0xFF,0x48,0x25,0xD9,0x57,0x50,0x57,0xDA,0x25,0x00,0x00,0x00,0x0F,0x00,0x08,0x0B,0x0D,0x09,0x0D,0x0B,0x08,0x00,/*"橙",1*/
0x40,0x40,0x42,0x42,0x42,0xF2,0x4A,0x46,0x42,0x40,0x40,0x00,0x00,0x00,0x00,0x08,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,/*"子",2*/
0x08,0x90,0xFC,0x04,0xF4,0x55,0x96,0x54,0x14,0xF4,0x04,0x00,0x09,0x04,0x0B,0x04,0x03,0x04,0x03,0x04,0x00,0x07,0x0C,0x00,/*"疯",3*/
0x01,0xF9,0x09,0x89,0x7F,0x09,0x7F,0x89,0x89,0xF9,0x01,0x00,0x00,0x0F,0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x00, //"西",0x0
0x2C,0x24,0xA4,0x64,0x3D,0x26,0x24,0x24,0xE4,0x24,0x2C,0x00,0x08,0x08,0x09,0x05,0x05,0x02,0x02,0x05,0x04,0x08,0x00,0x00, //"安",0x1
0x10,0x11,0xF2,0x00,0x28,0xA4,0x2A,0xF9,0x2A,0xA4,0x28,0x00,0x08,0x04,0x03,0x04,0x09,0x08,0x0A,0x0B,0x08,0x08,0x09,0x00, //"途",0x2
0x02,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x08,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,0x00, //"丁",0x3
};

/**
  * @brief      6 * 12 ASCII字符集点阵.
  */
const unsigned char ASCII_6X12_DATA[] =	
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,/*"!",1*/
0x00,0x04,0x03,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*""",2*/
0x90,0xFC,0x90,0xFC,0x90,0x00,0x00,0x03,0x00,0x03,0x00,0x00,/*"#",3*/
0x18,0x24,0xFE,0x44,0x88,0x00,0x01,0x02,0x07,0x02,0x01,0x00,/*"$",4*/
0x38,0x44,0xF8,0xD0,0x2C,0xC0,0x00,0x03,0x00,0x01,0x02,0x01,/*"%",5*/
0xC0,0x38,0xE4,0x18,0xE0,0x20,0x01,0x02,0x02,0x01,0x02,0x01,/*"&",6*/
0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"'",7*/
0x00,0x00,0xF8,0x06,0x01,0x00,0x00,0x00,0x00,0x03,0x04,0x00,/*"(",8*/
0x00,0x01,0x06,0xF8,0x00,0x00,0x00,0x04,0x03,0x00,0x00,0x00,/*")",9*/
0x90,0x60,0xF8,0x60,0x90,0x00,0x00,0x00,0x01,0x00,0x00,0x00,/*"*",10*/
0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,/*"+",11*/
0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x03,0x00,0x00,0x00,0x00,/*",",12*/
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,/*"-",13*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,/*".",14*/
0x00,0x00,0xC0,0x30,0x0C,0x02,0x04,0x03,0x00,0x00,0x00,0x00,/*"/",15*/
0xF8,0x04,0x04,0x04,0xF8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"0",16*/
0x00,0x08,0xFC,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,/*"1",17*/
0x18,0x84,0x44,0x24,0x18,0x00,0x03,0x02,0x02,0x02,0x02,0x00,/*"2",18*/
0x08,0x04,0x24,0x24,0xD8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"3",19*/
0xC0,0xA0,0x98,0xFC,0x80,0x00,0x00,0x00,0x02,0x03,0x02,0x00,/*"4",20*/
0x7C,0x24,0x24,0x24,0xC4,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"5",21*/
0xF0,0x48,0x24,0x24,0xC8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"6",22*/
0x00,0x04,0xC4,0x34,0x0C,0x00,0x00,0x00,0x03,0x00,0x00,0x00,/*"7",23*/
0xD8,0x24,0x24,0x24,0xD8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"8",24*/
0x38,0x44,0x44,0x24,0xF8,0x00,0x01,0x02,0x02,0x01,0x00,0x00,/*"9",25*/
0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,/*":",26*/
0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,/*";",27*/
0x00,0x60,0x90,0x08,0x04,0x00,0x00,0x00,0x00,0x01,0x02,0x00,/*"<",28*/
0x50,0x50,0x50,0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00,0x00,/*"=",29*/
0x00,0x04,0x08,0x90,0x60,0x00,0x00,0x02,0x01,0x00,0x00,0x00,/*">",30*/
0x18,0x04,0xC4,0x24,0x18,0x00,0x00,0x00,0x02,0x00,0x00,0x00,/*"?",31*/
0xF0,0x08,0xE4,0xF4,0x84,0x78,0x00,0x01,0x02,0x02,0x02,0x01,/*"@",32*/
0x00,0xE0,0x9C,0xF0,0x80,0x00,0x02,0x03,0x00,0x00,0x03,0x02,/*"A",33*/
0x04,0xFC,0x24,0x24,0xD8,0x00,0x02,0x03,0x02,0x02,0x01,0x00,/*"B",34*/
0xF8,0x04,0x04,0x04,0x0C,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"C",35*/
0x04,0xFC,0x04,0x04,0xF8,0x00,0x02,0x03,0x02,0x02,0x01,0x00,/*"D",36*/
0x04,0xFC,0x24,0x74,0x0C,0x00,0x02,0x03,0x02,0x02,0x03,0x00,/*"E",37*/
0x04,0xFC,0x24,0x74,0x0C,0x00,0x02,0x03,0x02,0x00,0x00,0x00,/*"F",38*/
0xF0,0x08,0x04,0x44,0xCC,0x40,0x00,0x01,0x02,0x02,0x01,0x00,/*"G",39*/
0x04,0xFC,0x20,0x20,0xFC,0x04,0x02,0x03,0x00,0x00,0x03,0x02,/*"H",40*/
0x04,0x04,0xFC,0x04,0x04,0x00,0x02,0x02,0x03,0x02,0x02,0x00,/*"I",41*/
0x00,0x04,0x04,0xFC,0x04,0x04,0x0C,0x08,0x08,0x07,0x00,0x00,/*"J",42*/
0x04,0xFC,0x24,0x50,0x8C,0x04,0x02,0x03,0x02,0x00,0x03,0x02,/*"K",43*/
0x04,0xFC,0x04,0x00,0x00,0x00,0x02,0x03,0x02,0x02,0x02,0x03,/*"L",44*/
0xFC,0x3C,0xC0,0x3C,0xFC,0x04,0x03,0x00,0x03,0x00,0x03,0x02,/*"M",45*/
0x04,0xFC,0x30,0xC4,0xFC,0x04,0x02,0x03,0x02,0x00,0x03,0x00,/*"N",46*/
0xF8,0x04,0x04,0x04,0xF8,0x00,0x01,0x02,0x02,0x02,0x01,0x00,/*"O",47*/
0x04,0xFC,0x24,0x24,0x18,0x00,0x02,0x03,0x02,0x00,0x00,0x00,/*"P",48*/
0xF8,0x84,0x84,0x04,0xF8,0x00,0x01,0x02,0x02,0x07,0x05,0x00,/*"Q",49*/
0x04,0xFC,0x24,0x64,0x98,0x00,0x02,0x03,0x02,0x00,0x03,0x02,/*"R",50*/
0x18,0x24,0x24,0x44,0x8C,0x00,0x03,0x02,0x02,0x02,0x01,0x00,/*"S",51*/
0x0C,0x04,0xFC,0x04,0x0C,0x00,0x00,0x02,0x03,0x02,0x00,0x00,/*"T",52*/
0x04,0xFC,0x00,0x00,0xFC,0x04,0x00,0x01,0x02,0x02,0x01,0x00,/*"U",53*/
0x04,0x7C,0x80,0xE0,0x1C,0x04,0x00,0x00,0x03,0x00,0x00,0x00,/*"V",54*/
0x3C,0xC0,0x7C,0xC0,0x3C,0x00,0x00,0x03,0x00,0x03,0x00,0x00,/*"W",55*/
0x04,0x9C,0x60,0x9C,0x04,0x00,0x02,0x03,0x00,0x03,0x02,0x00,/*"X",56*/
0x04,0x3C,0xC0,0x3C,0x04,0x00,0x00,0x02,0x03,0x02,0x00,0x00,/*"Y",57*/
0x0C,0x84,0x64,0x1C,0x04,0x00,0x02,0x03,0x02,0x02,0x03,0x00,/*"Z",58*/
0x00,0x00,0xFF,0x01,0x01,0x00,0x00,0x00,0x07,0x04,0x04,0x00,/*"[",59*/
0x00,0x06,0x38,0xC0,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x00,/*"\",60*/
0x00,0x01,0x01,0xFF,0x00,0x00,0x00,0x04,0x04,0x07,0x00,0x00,/*"]",61*/
0x00,0x02,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"^",62*/
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,/*"_",63*/
0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"`",64*/
0x00,0x40,0xA0,0xA0,0xC0,0x00,0x00,0x01,0x02,0x02,0x03,0x02,/*"a",65*/
0x02,0xFE,0x20,0x20,0xC0,0x00,0x00,0x03,0x02,0x02,0x01,0x00,/*"b",66*/
0x00,0xC0,0x20,0x20,0x60,0x00,0x00,0x01,0x02,0x02,0x01,0x00,/*"c",67*/
0x00,0xC0,0x20,0x22,0xFE,0x00,0x00,0x01,0x02,0x02,0x03,0x02,/*"d",68*/
0x00,0xC0,0xA0,0xA0,0xC0,0x00,0x00,0x01,0x02,0x02,0x02,0x00,/*"e",69*/
0x00,0x20,0xFC,0x22,0x22,0x04,0x00,0x02,0x03,0x02,0x02,0x00,/*"f",70*/
0x00,0x40,0xA0,0xA0,0x60,0x20,0x00,0x05,0x0A,0x0A,0x0A,0x04,/*"g",71*/
0x02,0xFE,0x20,0x20,0xC0,0x00,0x02,0x03,0x02,0x00,0x03,0x02,/*"h",72*/
0x00,0x20,0xE6,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,/*"i",73*/
0x00,0x00,0x20,0xE6,0x00,0x00,0x08,0x08,0x08,0x07,0x00,0x00,/*"j",74*/
0x02,0xFE,0x80,0x60,0x20,0x00,0x02,0x03,0x00,0x01,0x02,0x00,/*"k",75*/
0x02,0x02,0xFE,0x00,0x00,0x00,0x02,0x02,0x03,0x02,0x02,0x00,/*"l",76*/
0xE0,0x20,0xE0,0x20,0xC0,0x00,0x03,0x00,0x03,0x00,0x03,0x00,/*"m",77*/
0x20,0xE0,0x20,0x20,0xC0,0x00,0x02,0x03,0x02,0x00,0x03,0x02,/*"n",78*/
0x00,0xC0,0x20,0x20,0xC0,0x00,0x00,0x01,0x02,0x02,0x01,0x00,/*"o",79*/
0x20,0xE0,0x20,0x20,0xC0,0x00,0x08,0x0F,0x0A,0x02,0x01,0x00,/*"p",80*/
0x00,0xC0,0x20,0x20,0xE0,0x00,0x00,0x01,0x02,0x0A,0x0F,0x08,/*"q",81*/
0x20,0xE0,0x40,0x20,0x20,0x00,0x02,0x03,0x02,0x00,0x00,0x00,/*"r",82*/
0x00,0x60,0xA0,0xA0,0x20,0x00,0x00,0x02,0x02,0x02,0x03,0x00,/*"s",83*/
0x00,0x20,0xF8,0x20,0x20,0x00,0x00,0x00,0x03,0x02,0x02,0x00,/*"t",84*/
0x20,0xE0,0x00,0x20,0xE0,0x00,0x00,0x01,0x02,0x02,0x03,0x02,/*"u",85*/
0x20,0xE0,0x00,0xE0,0x20,0x00,0x00,0x00,0x03,0x00,0x00,0x00,/*"v",86*/
0x60,0x80,0xE0,0x80,0x60,0x00,0x00,0x03,0x00,0x03,0x00,0x00,/*"w",87*/
0x20,0x60,0x80,0x60,0x20,0x00,0x02,0x03,0x00,0x03,0x02,0x00,/*"x",88*/
0x20,0xE0,0x00,0x00,0xE0,0x20,0x08,0x08,0x05,0x03,0x00,0x00,/*"y",89*/
0x00,0x20,0xA0,0x60,0x20,0x00,0x00,0x02,0x03,0x02,0x02,0x00,/*"z",90*/
0x00,0x00,0x20,0xFF,0x01,0x00,0x00,0x00,0x00,0x07,0x04,0x00,/*"{",91*/
0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,/*"|",92*/
0x00,0x01,0xDF,0x20,0x00,0x00,0x00,0x04,0x07,0x00,0x00,0x00,/*"}",93*/
0x02,0x01,0x01,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"~",94*/
};
