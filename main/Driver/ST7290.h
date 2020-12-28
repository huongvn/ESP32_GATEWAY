#ifndef _ST7290_H_
#define _ST7290_H_

/* The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use VSPI, leaving HSPI unused.
 * 
 * However if we simply intialise two instance of the SPI class for both
 * of these buses both can be used. However when just using these the Arduino
 * way only will actually be outputting at a time.
 */
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

uint8_t startRow, startCol, endRow, endCol; // coordinates of the dirty rectangle
uint8_t numRows = 64;
uint8_t numCols = 128;
uint8_t Graphic_Check = 0;

//GLCD Buf
uint8_t GLCD_Buf[1024];

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
void sendCmd(byte b) 
{
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(LCD_CS, HIGH);
  vspi->transfer(0xF8);
  vspi->transfer(b & 0xF0);
  vspi->transfer(b << 4);
  digitalWrite(LCD_CS, LOW);
  vspi->endTransaction();
}
void sendData(byte b) 
{
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(LCD_CS, HIGH);
  vspi->transfer(0xFA);
  vspi->transfer(b & 0xF0);
  vspi->transfer(b << 4);
  digitalWrite(LCD_CS, LOW);
  vspi->endTransaction();
}
void ST7290_init() 
{
  sendCmd(LCD_BASIC);
  sendCmd(LCD_BASIC);
  sendCmd(LCD_CLS); vTaskDelay(2);
  sendCmd(LCD_ADDRINC);
  sendCmd(LCD_DISPLAYON);
}
void vspiCommand() {
  byte data = 0b01010101; // junk data to illustrate usage

  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer(data);  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
}

void LCD_setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  //clock miso mosi ss
  //initialise vspi with default pins
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  vspi->begin();
  pinMode(LCD_CS, OUTPUT); //VSPI SS
  digitalWrite(LCD_CS, LOW);
  ST7290_init();
}

void LCD_SendString(int row, int col, char* string){
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0x90;
            break;
        case 2:
            col |= 0x88;
            break;
        case 3:
            col |= 0x98;
            break;
        default:
            col |= 0x80;
            break;
    }
    SendCmd(col);
    while (*string)
    	{
    		SendData(*string++);
    	}
}

void LCD_GraphicMode (int enable){   // 1-enable, 0-disable{
	if (enable == 1){
		SendCmd(0x30);  // 8 bit mode
		vTaskDelay (1);
		SendCmd(0x34);  // switch to Extended instructions
		vTaskDelay (1);
		SendCmd(0x36);  // enable graphics
		vTaskDelay (1);
		Graphic_Check = 1;  // update the variable
	}

	else if (enable == 0){
		SendCmd(0x30);  // 8 bit mode
		vTaskDelay (1);
		Graphic_Check = 0;  // update the variable
	}
}
void LCD_DrawBitmap(const unsigned char* graphic)
{
	
	uint8_t x, y;
	
	uint16_t Index=0;
	uint8_t Temp,Db;
	
	for(y=0;y<64;y++)
	{
		for(x=0;x<8;x++)
		{
			if(y<32)//Up
			{
				SendCmd(0x80 | y);										//Y(0-31)
				SendCmd(0x80 | x);										//X(0-8)
			}
			else
			{
				SendCmd(0x80 | y-32);//Y(0-31)
				SendCmd(0x88 | x);//X(0-8)
			}
			
			Index=((y/8)*128)+(x*16);
			Db=y%8;			
			
			Temp=	(((graphic[Index+0]>>Db)&0x01)<<7)|
						(((graphic[Index+1]>>Db)&0x01)<<6)|
						(((graphic[Index+2]>>Db)&0x01)<<5)|
						(((graphic[Index+3]>>Db)&0x01)<<4)|
						(((graphic[Index+4]>>Db)&0x01)<<3)|
						(((graphic[Index+5]>>Db)&0x01)<<2)|
						(((graphic[Index+6]>>Db)&0x01)<<1)|
						(((graphic[Index+7]>>Db)&0x01)<<0);
			sendData(Temp);
			
			Temp=	(((graphic[Index+8]>>Db)&0x01)<<7)|
						(((graphic[Index+9]>>Db)&0x01)<<6)|
						(((graphic[Index+10]>>Db)&0x01)<<5)|
						(((graphic[Index+11]>>Db)&0x01)<<4)|
						(((graphic[Index+12]>>Db)&0x01)<<3)|
						(((graphic[Index+13]>>Db)&0x01)<<2)|
						(((graphic[Index+14]>>Db)&0x01)<<1)|
						(((graphic[Index+15]>>Db)&0x01)<<0);
			
			sendData(Temp);
		}
	}		
}

#endif 


