/**
 * @file demo_hello_world.cpp
 * @brief Demo: Schreibt "Hello World!" mit dem Plotter
 */

#include "demo_hello_world.h"
#include "../tiny_turtle.h"
#include "esp_log.h"

static const char* TAG = "demo_hello_world";

namespace tiny_turtle {
namespace demos {

void runHelloWorld(float height) {
    ESP_LOGI(TAG, "=== Hello World Demo ===");
    ESP_LOGI(TAG, "Zeichenhöhe: %.1f mm", height);
    
    // Text schreiben
    drawing::plotText("Hello World! ", height);
    
    ESP_LOGI(TAG, "Demo beendet.");
}

}  // namespace demos
}  // namespace tiny_turtle
