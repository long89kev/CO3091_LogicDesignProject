#include "global.h"

float glob_temp = 0;
float glob_humid = 0;
int glob_light = 0;
float glob_soil = 0;
float glob_rain = 0;

SemaphoreHandle_t xSensorMutex = NULL;
SemaphoreHandle_t xSerialMutex = NULL;
SemaphoreHandle_t xMqttMutex = NULL;

uint8_t g_pumpControl = 0;
uint32_t g_pumpDuration = 0;
uint8_t g_pumpMode = 0;