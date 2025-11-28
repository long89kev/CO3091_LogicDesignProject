#include "taskWiFi.h"

static const char *WIFI_SSID = "CALLMECHUNG";
static const char *WIFI_PASS = "28032005";

void WiFi_Connect()
{
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    uint32_t t = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t < 5000)
    {
        Serial.print(".");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("\nWiFi connect time out, retry later...");
    }
    else
    {
        Serial.print("\nWiFi connected!\n");
    }
}

void task_WiFi(void *pvParameter)
{

    bool logged = false;

    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            logged = false;

            if (xSerialMutex != NULL &&
                xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
            {
                Serial.print("WiFi not connected, start connecting");
                WiFi_Connect();
                xSemaphoreGive(xSerialMutex);
            }
        }
        else if (!logged)
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                if (xSerialMutex != NULL &&
                    xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
                {
                    Serial.print("WiFi connected, IP: ");
                    Serial.println(WiFi.localIP());
                    xSemaphoreGive(xSerialMutex);
                }
            }

            logged = true;
        }
        else if (logged)
        {
            if (xSerialMutex != NULL &&
                xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
            {
                Serial.println("WiFi still connected");
                xSemaphoreGive(xSerialMutex);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}