#include "global.h"
#include "temp_humid_sensor.h"
#include "monitor_update.h"
#include "soil_sensor.h"
#include "taskWiFi.h"
#include "rain_sensor.h"
#include "water_sensor.h"

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

  xTaskCreate(monitor_update, "Task Printing", 2048, NULL, 1, NULL);
  xTaskCreate(temp_humid_sensor, "Task DHT", 4096, NULL, 2, NULL);
  xTaskCreate(soil_sensor, "Task Soil Sensor", 4096, NULL, 2, NULL);
  xTaskCreate(rain_sensor, "Task Rain Sensor", 2048, NULL, 2, NULL);
  xTaskCreate(water_sensor, "Task Water Flow Sensor", 4096, NULL, 2, NULL);
  xTaskCreate(task_WiFi, "Task WiFi", 4096, NULL, 3, NULL);
  

}

void loop() {}