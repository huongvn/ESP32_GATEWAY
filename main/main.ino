
#define ARDUINO_RUNNING_CORE 1
#define ARDUINO_ZERO_CORE 0

#include <Arduino.h>
#include <U8g2lib.h>
#include "WiFi.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
#define LCD_CLK 18 //GPIO 18
#define LCD_DATA 21 //GPIO21
#define LCD_CS 05 //GPIO05

#define WIFI_NETWORK "TuyenHaT1"
#define WIFI_PASSWORD "12345654"
#define WIFI_CHECKTIME 10000
#define WIFI_TIMEOUT_MS 20000
#define WIFI_RECOVER_TIME_MS 30000

// define task
void TaskWiFi( void *pvParameters );
void TaskDisplay( void *pvParameters );
void TaskStatusLed( void *pvParameters);
// setup lcd and lcd function
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ LCD_CLK /* A4 */ , /* data=*/ LCD_DATA /* A2 */, /* CS=*/ LCD_CS /* A3 */, /* reset=*/ U8X8_PIN_NONE);
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
// End lcd fucntion
// WiFi function 

// End WiFi function
// Begin lcd screen
void mainScreen(){
  u8g2.drawStr( 0, 0, "test");
  }
void WifiScreen(){
  
  }
// End lcd screen
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
  WiFi.mode(WIFI_STA);
  u8g2.begin();
  u8g2_prepare();
  xTaskCreatePinnedToCore(TaskWiFi, "TaskWiFi", 4096, NULL, 1, NULL, ARDUINO_ZERO_CORE);
  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 4096, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskStatusLed, "TaskStatusLed", 1024, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  Serial.println("Setup done");
}
// loop loop 
void loop()
{
  // Do not place code here
}
/*---------------------- Tasks ---------------------*/
void TaskStatusLed(void *pvParameters){
    (void) pvParameters;
    char uxHighWaterMark;
    pinMode(LED_BUILTIN,OUTPUT);
    for (;;) {
      statusLed(05,5000);
      Serial.println("DEBUG: STATUS LED TASK");
      vTaskDelay(1);
      }
  }
void TaskWiFi(void *pvParameters){
    (void) pvParameters;
    for (;;) {
      int n = WiFi.scanNetworks();
      Serial.println("scan done");
      if (n == 0) {
        Serial.println("no networks found");
      } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
      }
      Serial.println("");
      vTaskDelay(5000);
        }
  }
void TaskDisplay(void *pvParameters){ // Run Diskplay Task
    (void) pvParameters;
    for(;;) {
      char *string_list = "aab\n";
      u8g2.userInterfaceSelectionList("NETWORK FOUND", 1, string_list);
      vTaskDelay(20);
      Serial.println("DEBUG: EXIT TASK DISPLAY");
      vTaskSuspend( NULL );
      }
  }
