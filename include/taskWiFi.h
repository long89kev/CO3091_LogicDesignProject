#ifndef __TASK_WIFI_H__
#define __TASK_WIFI_H__

#include "global.h"
#include <WiFi.h>

void WiFi_Connect();
void task_WiFi(void *pvParameter);

#endif