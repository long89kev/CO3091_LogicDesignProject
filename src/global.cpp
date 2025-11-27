#include "global.h"

float glob_temp = 0;
float glob_humid = 0;
int glob_light = 0;
float glob_soil= 0;

SemaphoreHandle_t xSensorMutex = NULL;
SemaphoreHandle_t xSerialMutex = NULL;