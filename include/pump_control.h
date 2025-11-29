#ifndef __PUMP_CONTROL_H__
#define __PUMP_CONTROL_H__

#include "global.h"

// void pump_control(void *pvParameter);

void pump_init();
void pump_start(uint32_t durationMs, const char *mode);
void pump_stop(uint32_t runTimeMs);
void pump_set_mode(const char *mode);

#endif