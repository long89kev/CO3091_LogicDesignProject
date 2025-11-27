#include "soil_sensor.h"

void soil_sensor(void *pvParameter)
{
    int soil_val = 0;
    int curr_soil_per = 0;

    while (1)
    {
        soil_val = analogRead(SOIL_PIN);

        curr_soil_per = map(soil_val, 4095, 0, 0, 100);

        if (xSensorMutex != NULL &&
            xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdTRUE)
        {
            glob_soil = curr_soil_per;
            xSemaphoreGive(xSensorMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}