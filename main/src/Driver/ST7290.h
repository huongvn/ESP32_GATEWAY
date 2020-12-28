#ifndef _ST7290_H_
#define _ST7290_H_

#include <Arduino.h>
#include <SPI.h>

static const int spiClk = 100000; // 0.2 MHz

#define LCD_CS          5
#define LCD_CLS         0x01
#define LCD_HOME        0x02
#define LCD_ADDRINC     0x06
#define LCD_DISPLAYON   0x0C
#define LCD_CURSORON    0x0E
#define LCD_CURSORBLINK 0x0F
#define LCD_BASIC       0x30
#define LCD_EXTEND      0x34
#define LCD_LINE0       0x80
#define LCD_LINE1       0x90
#define LCD_LINE2       0x88
#define LCD_LINE3       0x98


void sendCmd(byte b);
void sendData(byte b);
void ST7290_init(void);
void vspiCommand(void);
void LCD_setup(void);

void LCD_SendString(int row, int col, char* string);
void LCD_GraphicMode (int enable);   // 1-enable, 0-disable
void LCD_DrawBitmap(const unsigned char* graphic);

#endif 


