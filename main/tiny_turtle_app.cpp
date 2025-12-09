#include "tiny_turtle/tiny_turtle.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern void setup();
extern void loop();

extern "C" void app_main(void) {
  setup();
  while (true) {
    loop();
    vTaskDelay(1);  // yield to RTOS
  }
}
