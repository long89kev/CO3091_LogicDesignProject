#ifndef __BLINK_H___
#define __BLINK_H___

#include "global.h"



namespace blinky
{
    void begin();
    void on(TickType_t ms);
    void off(TickType_t ms);
};

void led_blinky(void *pvParameter);



#endif