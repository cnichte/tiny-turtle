#include "tiny_turtle/tiny_turtle.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void) {
  turtleInit();
  while (true) {
    turtleLoop();
    vTaskDelay(1);  // yield to RTOS
  }
}
