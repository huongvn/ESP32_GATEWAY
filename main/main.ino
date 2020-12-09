#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// the setup function
void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(TaskWifi, "TaskWifi", 1024, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
}

void loop()
{
  // Empty
}
/*---------------------- Tasks ---------------------*/
void TaskWifi(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) 
  {
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay(10); 
    digitalWrite(LED_BUILTIN, LOW);   
    vTaskDelay(1000);  
  }
}
