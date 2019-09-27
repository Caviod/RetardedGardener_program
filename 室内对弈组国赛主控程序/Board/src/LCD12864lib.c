#include "common.h"
#include "LCD12864lib.h"


const unsigned char cl[] = { /* 0X22,0X01,0X5A,0X00,0X40,0X00, *///����Ƭ
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XF0,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF8,0XE0,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X20,0X20,0X20,0X30,0X30,0X30,
0X78,0X70,0X70,0XF0,0XF0,0XF8,0XF8,0XF8,0XFC,0XFC,0XFC,0XFC,0XFC,0XF8,0XF0,0XF0,
0XE0,0XE0,0XC0,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0X0F,
0X07,0X03,0X01,0X01,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X90,0XC0,0X00,0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X01,
0X07,0X07,0X0F,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFC,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X80,0X00,0X00,0X00,0XC0,0XFC,0X06,0X01,0X00,0X00,0X00,0XC0,0X00,0X00,0XD8,0X04,
0X00,0X00,0X00,0X80,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,
0X00,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XA0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF8,0X80,0X00,0X00,0X00,0X00,0X08,0X00,0X10,0X3D,0X3F,0X7E,0X7C,0XF8,0XFF,0XFF,
0XFE,0XE0,0XC0,0X80,0X00,0X2F,0X38,0X1C,0X3F,0X3F,0X00,0X00,0X00,0X01,0X0B,0X00,
0X08,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XC0,0X00,0X00,0X00,0XE0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF0,0XE0,0XF0,
0XF8,0XF8,0XFE,0XFE,0XFC,0XFC,0XFC,0X3E,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,
0X0E,0X1C,0X1C,0X38,0X30,0X30,0X70,0X60,0XE0,0XE0,0XC0,0X40,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X0E,0X00,0X00,0X30,0X7F,0XFF,0XFC,0XFC,0XFC,0XFE,0XFC,0XFC,0XFC,
0XFC,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X3F,0X0F,0X03,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3F,0X1F,
0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X01,0X01,0X03,0X03,0X03,0X06,0X06,0X0E,0X0E,0X0E,0X0F,0X0F,0X03,0X03,
0X03,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,0X07,
0X07,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
