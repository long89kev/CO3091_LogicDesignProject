#include "photodiode.h"

const int ACTIVE_LEVEL = LOW;

const unsigned long debounceDelay = 20; // 20 ms

void light_sensor(void *pvParameter)
{
    pinMode(PHOTODIODE_PIN, INPUT); 

    int stableState = digitalRead(PHOTODIODE_PIN);
    int lastReading = stableState;
    int current_light = (stableState == ACTIVE_LEVEL) ? 1 : 0;

    unsigned long lastDebounceTime = 0;

    while (1)
    {
        int reading = digitalRead(PHOTODIODE_PIN);
        unsigned long now = millis();

        if (reading != lastReading)
        {
            lastDebounceTime = now; // bắt đầu lại thời gian debounce
        }

        if ((now - lastDebounceTime) > debounceDelay)
        {
            if (reading != stableState)
            {
                stableState = reading;

                // Có tia IR / ánh sáng -> ACTIVE_LEVEL
                if (stableState == ACTIVE_LEVEL)
                    current_light = 1;
                else
                    current_light = 0;

                if (xSensorMutex != NULL &&
                    xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdPASS)
                {
                    glob_light = current_light; // 0 = off, 1 = on
                    xSemaphoreGive(xSensorMutex);
                }
            }
        }

        lastReading = reading;

        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}
