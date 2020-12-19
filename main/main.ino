#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <Arduino.h>
#include <U8g2lib.h>
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// define task
void TaskWifi( void *pvParameters );
void TaskDisplay( void *pvParameters );
void TaskStatusLed( void *pvParameters);
// setup lcd and lcd function
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 21 /* A2 */, /* CS=*/ 05 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}
void u8g2_ascii_1() {
  char s[2] = " ";
  uint8_t x, y;
  u8g2.drawStr( 0, 0, "ASCII page 1");
  for( y = 0; y < 6; y++ ) {
    for( x = 0; x < 16; x++ ) {
      s[0] = y*16 + x + 32;
      u8g2.drawStr(x*7, y*10+10, s);
    }
  }
}
void mainScreen(){
  u8g2.drawStr( 0, 0, "test");
  }
// Status led 
void statusLed(int ton,int toff){
  digitalWrite(LED_BUILTIN, HIGH);   
  vTaskDelay(ton); 
  digitalWrite(LED_BUILTIN, LOW);   
  vTaskDelay(toff);  
  }

// the setup function
void setup() {
  Serial.begin(115200);
  u8g2.begin();

  xTaskCreatePinnedToCore(TaskWifi, "TaskWifi", 1024, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 1024, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskStatusLed, "TaskStatusLed", 512, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  
}
// loop loop 
void loop()
{
  // Do not work
}
/*---------------------- Tasks ---------------------*/
void TaskStatusLed(void *pvParameters){
    (void) pvParameters;
    char uxHighWaterMark;
    pinMode(LED_BUILTIN,OUTPUT);
    for (;;) {
      statusLed(05,5000);
      vTaskDelay(1);
      }
  }
void TaskWifi(void *pvParameters){
    (void) pvParameters;
    for (;;) {
      vTaskDelay(1);
      }
  }
void TaskDisplay(void *pvParameters){ // Run Diskplay Task
    (void) pvParameters;
    char uxHighWaterMark;
    for(;;) {
      
      u8g2.clearBuffer();
      u8g2_prepare();
      u8g2_ascii_1();
      mainScreen();
      u8g2.sendBuffer();
      vTaskDelay(1000);
      }
    
  }
