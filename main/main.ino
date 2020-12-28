
#define ARDUINO_ONE_CORE 1  //APP_CODE
#define ARDUINO_ZERO_CORE 0 //PRO_CODE

#include "main.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// define task
void TaskWiFi( void *pvParameters );
void TaskDisplay( void *pvParameters );
void TaskStatusLed( void *pvParameters);

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
  LCD_setup();
  xTaskCreatePinnedToCore(TaskWiFi, "TaskWiFi", 4096, NULL, 1, NULL, ARDUINO_ONE_CORE);
  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 4096, NULL, 2, NULL, ARDUINO_ONE_CORE);
  xTaskCreatePinnedToCore(TaskStatusLed, "TaskStatusLed", 1024, NULL, 3, NULL, ARDUINO_ONE_CORE);
  Serial.println("DEBUG: Setup done");
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
      Serial.println("DEBUG: scan done");
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
      


      vTaskDelay(20);
      }
  }
