#include "update_value.h"

void increase(void *pvParameter)
{
    while (1)
    {
        glob_va++;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}