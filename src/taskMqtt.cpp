#include "taskMqtt.h"
#include <WiFi.h>
#include <PubSubClient.h> // MQTT library

static const char *MQTT_SERVER = "broker.hivemq.com";
static const uint16_t MQTT_PORT = 1883;

// Topic cho Pub/Sub
static const char *TOPIC_SENSOR = "esp32/sensors";
static const char *TOPIC_CMD = "esp32/pump_cmd";

// Client MQTT
static WiFiClient s_espClient;                 // TCP Client
static PubSubClient s_mqttClient(s_espClient); // vừa pub vừa sub

// Callback khi nhận payload message MQTT (SUB)
static void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
{
    // Copy payload sang 1 buffer có null-terminator để in cho dễ
    char buf[64];
    unsigned int n = (length < sizeof(buf) - 1) ? length : sizeof(buf) - 1;
    memcpy(buf, payload, n);
    buf[n] = '\0';

    if (xSerialMutex != NULL &&
        xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
    {
        Serial.print("[MQTT] Message arrived on topic: ");
        Serial.println(topic);
        Serial.print("Payload: ");
        Serial.println(buf);
        xSemaphoreGive(xSerialMutex);
    }
}

void MQTT_Connect()
{
    s_mqttClient.setServer(MQTT_SERVER, MQTT_PORT); // chỉ định broker: broker.hivemq.com:1883
    s_mqttClient.setCallback(mqttCallback);

    uint32_t t0 = millis();
    while (!s_mqttClient.connected() && millis() - t0 < 5000)
    {
        String clientID = "ESP32-" + String(random(0xffff), HEX);
        Serial.print("[MQTT] Connecting...");

        if (s_mqttClient.connect(clientID.c_str()))
        {
            Serial.println("OK");
            // Subribe topic
            s_mqttClient.subscribe(TOPIC_CMD);
        }
        else
        {
            Serial.print("Failed, reconnecting....");
            Serial.print(s_mqttClient.state());
            Serial.println(" try again...");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    if (!s_mqttClient.connected())
    {
        Serial.println("[MQTT] Connect timeout, retry later !!!");
    }
}

void task_MQTT(void *pvParameter)
{
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW);
    glob_pump_running = false;

    TickType_t lastWake = xTaskGetTickCount();

    while (1)
    {
        // 1. Chờ WiFI
        if (WiFi.status() != WL_CONNECTED)
        {
            if (xSerialMutex != NULL &&
                xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
            {
                Serial.println("[MQTT] Waiting for WiFi...");
                xSemaphoreGive(xSerialMutex);
            }

            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }

        // 2. Đảm bảo MQTT đã kết nối
        if (!s_mqttClient.connected())
        {
            if (xSerialMutex != NULL &&
                xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
            {
                MQTT_Connect();
                xSemaphoreGive(xSerialMutex);
            }
        }

        // 3. Nếu đã kết nối -> gọi loop xử lý gói đến
        if (s_mqttClient.connected())
        {
            s_mqttClient.loop();
        }

        // 4. Read sensor
        float temp, hum, soil, rain, total_ml;

        if (xSensorMutex != NULL &&
            xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdPASS)
        {
            temp = glob_temp;
            hum = glob_humid;
            soil = glob_soil;
            rain = glob_rain;
            total_ml = glob_total_ml;
            xSemaphoreGive(xSensorMutex);
        }

        // 5. Format payload
        char payload[128];
        snprintf(payload, sizeof(payload),
                 "{\"temp\":%.2f,\"hum\":%.2f,"
                 "\"soil\":%.2f,\"rain\":%.2f,\"water_ml\":%.2f}",
                 temp, hum, soil, rain, total_ml);

        if (s_mqttClient.connected())
        {
            s_mqttClient.publish(TOPIC_SENSOR, payload);
        }

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(5000));
    }
}