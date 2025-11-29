#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// ===== Sensors PINs =====
#define DHT_PIN         32     
#define DHT_TYPE        DHT22    

#define SOIL_PIN        34
#define RAIN_PIN        39

#define WATER_PIN       35

#define RAIN_PIN        39

#define PUMP_PIN        33

// === Global variable ===
extern float glob_temp;
extern float glob_humid; 

extern float glob_soil;
extern float glob_rain;

extern float glob_total_ml;
extern bool glob_pump_running;


// === Mutex ===
extern SemaphoreHandle_t xSensorMutex;
extern SemaphoreHandle_t xSerialMutex;
extern SemaphoreHandle_t xMqttMutex;

#endif