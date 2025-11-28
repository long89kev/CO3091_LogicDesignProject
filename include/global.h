#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// ===== Sensors PINs =====
#define DHT_PIN         32     
<<<<<<< HEAD
#define DHT_TYPE        DHT22    

=======
#define DHT_TYPE        DHT22
#define PHOTODIODE_PIN  36      
>>>>>>> 139d8acd4759b8faf4def99906d9a2782b21d002
#define SOIL_PIN        34
#define RAIN_PIN        39

<<<<<<< HEAD
#define WATER_PIN       35

#define RAIN_PIN        39

#define PUMP_PIN        33

// === Global variable ===
extern float glob_temp;
extern float glob_humid; 

=======
// ===== Global values =====
extern float glob_temp;
extern float glob_humid;
extern int glob_light; 
>>>>>>> 139d8acd4759b8faf4def99906d9a2782b21d002
extern float glob_soil;
extern float glob_rain;

<<<<<<< HEAD
extern float glob_total_ml;
extern bool glob_pump_running;

extern float glob_rain;

// === Mutex ===
extern SemaphoreHandle_t xSensorMutex;
extern SemaphoreHandle_t xSerialMutex;
extern SemaphoreHandle_t xMqttMutex;
=======
// ====== Mutex ======
extern SemaphoreHandle_t xSensorMutex;
extern SemaphoreHandle_t xSerialMutex;
extern SemaphoreHandle_t xMqttMutex;

// ====== Pump command ====
extern uint8_t g_pumpControl;       // 1 = START, 2 = STOP
extern uint32_t g_pumpDuration;     // ms
extern uint8_t g_pumpMode;          // auto/manual


>>>>>>> 139d8acd4759b8faf4def99906d9a2782b21d002

#endif