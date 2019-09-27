/***********************************************************************************


************************************************************************************/
#ifndef __LCD12864_H__
#define __LCD12864_H__

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void LCD_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_PutPixel(unsigned char x,unsigned char y);
extern void LCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
extern void Draw_LQLogo(void);
extern void Draw_LibLogo(void);
extern void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char * bmp);
extern void Draw_Road(void);
extern void LCD_show_image();
extern void LCD_show_ZZF_image();
extern void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);
extern void LCD_draw_bmp();
extern void LCD_PrintU16(unsigned char x,unsigned char y,unsigned int num);
extern void LCD_Print16(unsigned char x,unsigned char y,int num);
extern void OLED_Refresh_Gram(void);
extern void LCD_write_char(uint8 X, uint8 Y,uint16 c);
extern void LCD_Show_Number3 (uint8 X,uint8 Y,uint16 number) ;
extern void OLED_PrintFloat(unsigned char x , unsigned char y , float num);

#endif

