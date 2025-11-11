#include "global.h"
#include "monitor_update.h"
#include "update_value.h"


void setup()
{
  Serial.begin(115200);
  delay(500);

  xTaskCreate(monitor_update, "Task Printing", 2048, NULL, 1, NULL);
  xTaskCreate(increase, "Task increasing", 2048, NULL, 1, NULL);
}

void loop() {}