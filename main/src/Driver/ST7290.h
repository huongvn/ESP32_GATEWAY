#ifndef _ST7290_H_
#define _ST7290_H_

#include <Arduino.h>
#include <SPI.h>
#include "font.h"

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
void vspiCommand(void);
void GLCD_init(void);
void GLCD_setup(void);

void GLCD_SendString(int row, int col, char* string);
void GLCD_GraphicMode (int enable);   // 1-enable, 0-disable
void GLCD_DrawBitmap(const unsigned char* graphic);
void GLCD_Buf_Clear(void);
void GLCD_Update(void);
void GLCD_Clear(void);
void GLCD_Font_Print(uint8_t x,uint8_t y,char * String);
void GLCD_ICON_Print(uint8_t x,uint8_t y,const uint8_t * ICON);

void SetPixel(uint8_t x, uint8_t y);
void ClearPixel(uint8_t x, uint8_t y);
void TogglePixel(uint8_t x, uint8_t y);
void ClearFilledCircle(int16_t x0, int16_t y0, int16_t r);
void ClearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void ToggleLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void ToggleRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius);
void DrawFilledCircle(int16_t x0, int16_t y0, int16_t r);
void DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

#endif 


