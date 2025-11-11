#include "blink.h"

namespace blinky
{
    void begin()
    {
        pinMode(LED_PIN, OUTPUT);
    }

    void on(TickType_t ms)
    {
        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(ms));
    }

    void off(TickType_t ms)
    {
        digitalWrite(LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}; 

void led_blinky(void *pvParameter)
{
    static float current_temp = 0; 
    static float current_humid = 0;

    blinky::begin();

    while (1)
    {
        if (xSensorMutex != NULL &&
            xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdTRUE)
        {
            current_temp = glob_temp;
            current_humid = glob_humid;
            xSemaphoreGive(xSensorMutex);
        }

        if (current_temp < 20)
        {
            blinky::on(1000);
            blinky::off(2000);
        }
        else if (current_temp < 25)
        {
            blinky::on(500);
            blinky::off(1000);
        }
        else if (current_temp < 30)
        {
            blinky::on(450);
            blinky::off(50);
        }
        else
        {
            blinky::on(10000);
            blinky::off(500);
        }

        vTaskDelay(1000);
    }
}
