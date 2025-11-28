#include "monitor_update.h"

void monitor_update(void *pvParameter)
{
    float current_temp = 0;
    float current_humid = 0;
    float current_soil = 0;
    int current_light = 0;
    float current_rain = 0;

    while (1)
    {
        if (xSensorMutex != NULL &&
            xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdTRUE)
        {
            current_temp = glob_temp;
            current_humid = glob_humid;
            current_light = glob_light;
            current_soil = glob_soil;
            current_rain = glob_rain;
            xSemaphoreGive(xSensorMutex);
        }

        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdTRUE)
        {
            if (isnan(current_temp) || isnan(current_humid))
            {
                Serial.println("[DHT Sensor] is disconnected !!!");
                current_temp = -1;
                current_humid = -1;
            }

            if (isnan(current_soil))
            {
                Serial.println("[Soil Sensor] Sensor is disconnected !!!");
            }

            Serial.print("[DHT] Humidity: ");
            Serial.print(current_humid);
            Serial.print("%  Temperature: ");
            Serial.print(current_temp);
            Serial.println("°C");

            if (current_light > 0)
                Serial.println("Light is on");
            else
                Serial.println("Light is off");

            Serial.print("[SOIL SENSOR] measure: ");
            Serial.print(current_soil);
            Serial.println("%");

            Serial.print("[RAIN SENSOR] Weather: ");
            Serial.println(current_rain);

            Serial.print("\n");
            xSemaphoreGive(xSerialMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // nên dùng pdMS_TO_TICKS cho đồng bộ
    }
}