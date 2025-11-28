#include "global.h"

float glob_temp = 0;
float glob_humid = 0;
<<<<<<< HEAD
float glob_soil= 0;
float glob_total_ml = 0;
float glob_rain = 0;
bool glob_pump_running = false;

SemaphoreHandle_t xSensorMutex = NULL;
SemaphoreHandle_t xSerialMutex = NULL;
SemaphoreHandle_t xMqttMutex = NULL;
=======
int glob_light = 0;
float glob_soil = 0;
float glob_rain = 0;

SemaphoreHandle_t xSensorMutex = NULL;
SemaphoreHandle_t xSerialMutex = NULL;
SemaphoreHandle_t xMqttMutex = NULL;

uint8_t g_pumpControl = 0;
uint32_t g_pumpDuration = 0;
uint8_t g_pumpMode = 0;
>>>>>>> 139d8acd4759b8faf4def99906d9a2782b21d002
