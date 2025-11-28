#include "rain_sensor.h"

void rain_sensor(void *pvParameter)
{
    pinMode(RAIN_PIN, INPUT);

    while (1)
    {
        int rain_value = analogRead(RAIN_PIN);
        if (xSensorMutex != NULL &&
            xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdPASS)
        {
            glob_rain = rain_value;
            xSemaphoreGive(xSensorMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}