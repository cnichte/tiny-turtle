/**
 * @file demo_motor_test.cpp
 * @brief Demo: GPTimer Motor-Test mit Beschleunigung/Verzögerung (Rampen)
 */

#include "demo_motor_test.h"
#include "../tiny_turtle.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "demo_motor_test";

namespace tiny_turtle {
namespace demos {

void runMotorTest() {
    ESP_LOGI(TAG, "=== GPTimer Rampen-Demo ===");
    
    // Rampe aktivieren: 200 Schritte für Beschleunigung/Verzögerung
    hal::setRamp(200);
    
    // Startgeschwindigkeit setzen (langsam)
    hal::setStepSpeed(5000);  // 5000 µs = 200 Steps/s (Start)
    
    // Vorwärts starten
    ESP_LOGI(TAG, "Vorwärts mit Beschleunigung...");
    hal::setMotorCommand(MotorCommand::FORWARD);
    
    // Beschleunigung zu schnell
    hal::setTargetSpeed(1000);  // Rampe zu 1000 µs = 1000 Steps/s
    
    // Warten bis Rampe fertig + fahren
    while (hal::isRamping()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    ESP_LOGI(TAG, "Volle Geschwindigkeit erreicht!");
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    // Sanft stoppen (mit Verzögerungsrampe)
    ESP_LOGI(TAG, "Sanftes Stoppen...");
    hal::smoothStop();
    
    // Warten bis gestoppt
    while (hal::isRamping()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    ESP_LOGI(TAG, "Gestoppt!");
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // Drehung mit Rampe
    ESP_LOGI(TAG, "Drehung CW mit Rampe...");
    hal::setStepSpeed(4000);  // Langsam starten
    hal::setMotorCommand(MotorCommand::SPIN_CW);
    hal::setTargetSpeed(800);  // Sehr schnell werden
    
    while (hal::isRamping()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    ESP_LOGI(TAG, "Max Drehgeschwindigkeit!");
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    // Abbremsen auf mittlere Geschwindigkeit
    ESP_LOGI(TAG, "Abbremsen...");
    hal::setTargetSpeed(2500);
    
    while (hal::isRamping()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // Sanft stoppen
    hal::smoothStop();
    while (hal::isRamping()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    ESP_LOGI(TAG, "Motor-Test beendet.");
}

void runSimpleRotationTest() {
    ESP_LOGI(TAG, "=== Einfacher Rotationstest ===");
    ESP_LOGI(TAG, "Beide Räder drehen vorwärts (Dauerbetrieb)");
    
    // Feste Geschwindigkeit, keine Rampe
    hal::setRamp(0);
    hal::setStepSpeed(2000);  // 2000 µs = 500 Steps/s
    
    // Vorwärts starten
    hal::setMotorCommand(MotorCommand::FORWARD);
    
    ESP_LOGI(TAG, "Motoren laufen... (Ctrl+C zum Stoppen)");
    
    // Endlosschleife - Motoren laufen weiter
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "Läuft...");
    }
}

}  // namespace demos
}  // namespace tiny_turtle
