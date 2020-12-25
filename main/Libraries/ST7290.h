

/* The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use VSPI, leaving HSPI unused.
 * 
 * However if we simply intialise two instance of the SPI class for both
 * of these buses both can be used. However when just using these the Arduino
 * way only will actually be outputting at a time.
 * 
 * Logic analyser capture is in the same folder as this example as
 * "multiple_bus_output.png"
 * 
 * created 30/04/2018 by Alistair Symonds
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

#define 
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
void LCD_init() 
{
  //digitalWrite(LCD_RST, LOW);
  //vTaskDelay(100);
  //digitalWrite(LCD_RST, HIGH);

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

void ST7290setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);

  //clock miso mosi ss
  //initialise vspi with default pins
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  vspi->begin();
  pinMode(LCD_CS, OUTPUT); //VSPI SS
  digitalWrite(LCD_CS, LOW);
  LCD_init();
}
char buf[20];

void test8x16()
{
  int del = 2000;
  sendCmd(LCD_BASIC);
  unsigned int i,j,c=0;
  for(i=0;i<0x80;i++) {
    c++;
    if(c==16) sendCmd(LCD_LINE2);
    else if(c==32) sendCmd(LCD_LINE3);
    else if(c>=48 || i==0) {
      if(c>=48) vTaskDelay(del);
      snprintf(buf,20,"%02X..%02X",i,i+47);
      sendCmd(LCD_CLS); vTaskDelay(2);
      for(j=0;j<strlen(buf);j++) sendData(buf[j]);
      c=0;
      sendCmd(LCD_LINE1);
    }
    sendData(i==0?' ':i);
  }
  vTaskDelay(del);
}
void test16x16()
{
  int del = 200;
  sendCmd(LCD_BASIC);
  unsigned int i,j,c=0;
  int st=0xa140;
  for(i=st;i<0xffff;i++) {
    c++;
    if(c==8) sendCmd(LCD_LINE2);
    else if(c==16) sendCmd(LCD_LINE3);
    else if(c>=24 || i==st) {
      if(c>=24) vTaskDelay(i<0xac00?800:del);
      snprintf(buf,20,"%04X..%04X",i,i+23);
      sendCmd(LCD_CLS); vTaskDelay(2);
      for(j=0;j<strlen(buf);j++) sendData(buf[j]);
      c=0;
      sendCmd(LCD_LINE1);
    }
    sendData(i>>8);
    sendData(i&0xff);
  }
  vTaskDelay(del);
}
void menuHeader(){
    int del = 200;
    sendCmd(LCD_BASIC);
    sendCmd(LCD_LINE0);
    sendData(0x11);
    vTaskDelay(del);
}


