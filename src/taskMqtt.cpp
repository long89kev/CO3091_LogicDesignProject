#include "taskMqtt.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h> // MQTT library
#include "pump_control.h"

static const char *MQTT_SERVER = "broker.hivemq.com";
static const uint16_t MQTT_PORT = 1883;

// Topic cho Pub/Sub
static const char *TOPIC_SENSOR = "esp32/sensors";
static const char *TOPIC_CMD = "esp32/pump_cmd";

// Client MQTT
static WiFiClient s_espClient;                 // TCP Client
static PubSubClient s_mqttClient(s_espClient); // vừa pub vừa sub

// Callback khi nhận payload message MQTT (SUB)
// static void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
// {
//     // Copy payload sang 1 buffer có null-terminator để in cho dễ
//     char buf[128];
//     unsigned int n = (length < sizeof(buf) - 1) ? length : sizeof(buf) - 1;
//     memcpy(buf, payload, n);
//     buf[n] = '\0';

//     if (xSerialMutex != NULL &&
//         xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
//     {
//         Serial.print("[MQTT] Message arrived on topic: ");
//         Serial.println(topic);
//         Serial.print("Payload: ");
//         Serial.println(buf);
//         xSemaphoreGive(xSerialMutex);
//     }

// }

static void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
{
    // 1. Copy payload sang buffer có null-terminator
    char buf[256];
    unsigned int n = (length < sizeof(buf) - 1) ? length : sizeof(buf) - 1;
    memcpy(buf, payload, n);
    buf[n] = '\0';

    // 2. In ra Serial (có mutex)
    if (xSerialMutex != NULL &&
        xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
    {
        Serial.print("[MQTT] Message arrived on topic: ");
        Serial.println(topic);
        Serial.print("Payload: ");
        Serial.println(buf);
        xSemaphoreGive(xSerialMutex);
    }

    // 3. Parse JSON bằng ArduinoJson
    StaticJsonDocument<256> doc; // ESP32 đủ RAM cho size này

    DeserializationError err = deserializeJson(doc, buf);
    if (err)
    {
        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
        {
            Serial.print("[MQTT] JSON parse failed: ");
            Serial.println(err.c_str());
            xSemaphoreGive(xSerialMutex);
        }
        return;
    }

    // 4. Đọc field "command"
    const char *cmd = doc["command"] | "";
    if (cmd[0] == '\0')
    {
        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
        {
            Serial.println("[MQTT] Missing 'command' field");
            xSemaphoreGive(xSerialMutex);
        }
        return;
    }

    // 5. Đọc thêm timestamp (nếu có)
    uint32_t timestamp = doc["timestamp"] | 0;

    // 6. Xử lý theo từng loại command

    // ---- pump_start ----
    if (strcmp(cmd, "pump_start") == 0)
    {
        // {"command":"pump_start","timestamp":..., "duration":10,"durationMs":10000,"mode":"manual"}

        uint32_t duration = doc["duration"] | 0;     // giây
        uint32_t durationMs = doc["durationMs"] | 0; // ms
        const char *modeJson = doc["mode"] | "manual";

        if (durationMs == 0 && duration > 0)
        {
            durationMs = duration * 1000; // fallback: từ giây -> ms
        }

        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
        {
            Serial.println("[MQTT] Command: pump_start");
            Serial.print("  timestamp = ");
            Serial.println(timestamp);
            Serial.print("  durationMs = ");
            Serial.println(durationMs);
            Serial.print("  mode = ");
            Serial.println(modeJson);
            xSemaphoreGive(xSerialMutex);
        }

        pump_start(durationMs, modeJson);
    }
    // ---- pump_stop ----
    else if (strcmp(cmd, "pump_stop") == 0)
    {
        // {"command":"pump_stop","timestamp":..., "runTime":5000}

        uint32_t runTime = doc["runTime"] | 0;

        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
        {
            Serial.println("[MQTT] Command: pump_stop");
            Serial.print("  timestamp = ");
            Serial.println(timestamp);
            Serial.print("  runTime   = ");
            Serial.println(runTime);
            xSemaphoreGive(xSerialMutex);
        }

        pump_stop(runTime);
    }
    // ---- set_mode ----
    else if (strcmp(cmd, "set_mode") == 0)
    {
        // {"command":"set_mode","timestamp":..., "mode":"automatic"}

        const char *modeJson = doc["mode"] | "";

        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
        {
            Serial.println("[MQTT] Command: set_mode");
            Serial.print("  timestamp = ");
            Serial.println(timestamp);
            Serial.print("  mode      = ");
            Serial.println(modeJson);
            xSemaphoreGive(xSerialMutex);
        }

        pump_set_mode(modeJson);
    }
    else
    {
        // Command lạ
        if (xSerialMutex != NULL &&
            xSemaphoreTake(xSerialMutex, portMAX_DELAY) == pdPASS)
        {
            Serial.print("[MQTT] Unknown command: ");
            Serial.println(cmd);
            xSemaphoreGive(xSerialMutex);
        }
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
    pump_init();
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