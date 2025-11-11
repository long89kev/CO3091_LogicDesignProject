#include "monitor_update.h"


void temperature_update(float temp)
{
    if (temp < 20)
    {
        Serial.println("It is freezing");
    }
    else if (temp < 25)
    {
        Serial.println("It is cooling");
    }
    else if (temp < 30)
    {
        Serial.println("It is warming");
    }
    else
    {
        Serial.println("It is hot");
    }
}

void monitor_update(void *pvParameter)
{
    float current_temp = 0;
    float current_humid = 0;

    while (1)
    {
        if (xSensorMutex != NULL &&
            xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdTRUE)
        {
            current_temp = glob_temp;
            current_humid = glob_humid;
            xSemaphoreGive(xSensorMutex);
        }

        if (isnan(current_temp) || isnan(current_humid))
        {
            Serial.println("[DHT] Sensor is disconnected !!!");
            current_temp = -1;
            current_humid = -1;
        }
        else
        {
            temperature_update(current_temp);
        }

        Serial.print("[DHT] Humidity: ");
        Serial.print(current_temp);
        Serial.print("%  Temperature: ");
        Serial.print(current_humid);
        Serial.println("°C");

        vTaskDelay(1000);
    }
}