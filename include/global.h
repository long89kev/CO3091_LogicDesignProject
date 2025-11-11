#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LED_PIN     13
#define DHT_PIN     27 
#define DHT_TYPE    DHT11

extern float glob_temp;

#endif