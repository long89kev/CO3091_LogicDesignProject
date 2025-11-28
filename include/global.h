#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define DHT_PIN         32     
#define DHT_TYPE        DHT22

#define PHOTODIODE_PIN  36      

#define SOIL_PIN        34

#define RAIN_PIN        39

extern float glob_temp;
extern float glob_humid;

extern int glob_light; 

extern float glob_soil;

extern float glob_rain;

extern SemaphoreHandle_t xSensorMutex;
extern SemaphoreHandle_t xSerialMutex;

#endif