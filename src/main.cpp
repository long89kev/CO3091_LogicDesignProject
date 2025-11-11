#include "global.h"
#include "blink.h"
#include "temp_humid_sensor.h"
#include "monitor_update.h"


void setup()
{
  Serial.begin(115200);
  delay(3000);

  xSensorMutex = xSemaphoreCreateMutex();
  xSerialMutex = xSemaphoreCreateMutex();

  if (xSensorMutex == NULL || xSerialMutex == NULL)
  {
    Serial.println("Failed to create mutex!");
  }

  xTaskCreate(led_blinky, "Task LED", 2048, NULL, 1, NULL);
  xTaskCreate(temp_humid_sensor, "Task DHT", 4096, NULL, 1, NULL);
  xTaskCreate(monitor_update, "Task Printing", 2048, NULL, 1, NULL);

}

void loop() {}