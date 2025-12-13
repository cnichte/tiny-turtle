#include "tiny_turtle/tiny_turtle.h"
#include "tiny_turtle/demos/demos.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void)
{
  turtleInit();

  // Dauerhafter Motor-Test (Endlosschleife)
  tiny_turtle::demos::runSimpleRotationTest();

  while (true)
  {
    turtleLoop();
    vTaskDelay(1); // yield to RTOS
  }
}
