#include "water_sensor.h"

volatile long pulse_count;

void IRAM_ATTR pulse_counter() {
    pulse_count++;
}

void water_sensor(void *pvParameter) 
{
    pinMode(WATER_PIN, INPUT_PULLUP);

    bool is_measuring = false;
    unsigned long old_time = 0;

    while (1) 
    {
        bool system_pump_state = glob_total_ml;

        if (system_pump_state && !is_measuring) // started the pump
        {
            pulse_count = 0;
            attachInterrupt(digitalPinToInterrupt(WATER_PIN), pulse_counter, FALLING);
            old_time = millis();
            is_measuring = true;
        } else if (!system_pump_state && is_measuring) // stopped the pump
        {
            detachInterrupt(digitalPinToInterrupt(WATER_PIN));
            is_measuring = false;
        }

        if (is_measuring) 
        {
            if ((millis() - old_time > 200)) {
                detachInterrupt(digitalPinToInterrupt(WATER_PIN));

                unsigned long new_pulses = pulse_count;
                pulse_count = 0;

                attachInterrupt(digitalPinToInterrupt(WATER_PIN), pulse_counter, FALLING);
                old_time = millis();

                if (new_pulses > 0) {
                    float curr_ml = new_pulses * 2.22;

                    if (xSensorMutex != NULL &&
                        xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdTRUE)
                    {
                        glob_total_ml += curr_ml;
                        xSemaphoreGive(xSensorMutex);
                    }
                }
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        } else {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    } 
}