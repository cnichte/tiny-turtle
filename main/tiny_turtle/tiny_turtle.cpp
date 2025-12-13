/**
 * @file tiny_turtle.cpp
 * @brief Tiny Turtle Robot - Hauptimplementierung
 */

#include "tiny_turtle.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char* TAG = "tiny_turtle";

namespace tiny_turtle {

void init() {
    ESP_LOGI(TAG, "Tiny Turtle v%s initialisieren...", VERSION);
    
    // HAL initialisieren
    hal::initStepperTimer();
    hal::initServo();
    hal::initSensors();
    hal::initAudio();
    hal::initLed();
    
    // Stift hoch
    hal::penUp();
    
    // LED grün für "bereit"
    hal::setLedColor(0, 32, 0);
    
    // Kurzer Startton
    hal::playTone(1000, 100);
    vTaskDelay(pdMS_TO_TICKS(100));
    hal::playTone(1500, 100);
    
    ESP_LOGI(TAG, "Initialisierung abgeschlossen.");
}

void shutdown() {
    ESP_LOGI(TAG, "System herunterfahren...");
    
    // Motoren stoppen
    hal::stopMotors();
    
    // Stift hoch
    hal::penUp();
    
    // LED aus
    hal::ledOff();
    
    // Ton stoppen
    hal::stopTone();
    
    ESP_LOGI(TAG, "Shutdown abgeschlossen.");
}

void loop() {
    // Hauptschleife für nicht-FreeRTOS Nutzung
    // Bei FreeRTOS wird dies normalerweise nicht aufgerufen
    vTaskDelay(pdMS_TO_TICKS(10));
}

}  // namespace tiny_turtle

// ============================================================================
// Legacy-Kompatibilität
// ============================================================================

void turtleInit() {
    tiny_turtle::init();
}

void turtleLoop() {
    tiny_turtle::loop();
}
