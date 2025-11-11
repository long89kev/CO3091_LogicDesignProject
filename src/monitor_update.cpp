#include "monitor_update.h"

void monitor_update(void *pvParameter)
{
    while (1)
    {
        Serial.print("Current num = ");
        Serial.println(glob_va);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}