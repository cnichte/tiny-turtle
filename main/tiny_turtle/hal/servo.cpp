/**
 * @file hal/servo.cpp
 * @brief Implementierung der Servo-Steuerung (Stift)
 */

#include "servo.h"
#include "stepper.h"
#include "led.h"
#include "../core/config.h"
#include "../core/globals.h"
#include "gpio_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace tiny_turtle {
namespace hal {

// Servo-Instanz (aus Arduino-Compat)
static Servo s_servo;

void initServo() {
    // Servo wird bei Bedarf attached/detached
}

void penUp() {
    stopMotors();
    setLedColor(15, 15, 15);
    
    if (isDrawing) {
        s_servo.attach(config::SERVO_PIN);
        s_servo.write(config::SERVO_PEN_DOWN);
        vTaskDelay(pdMS_TO_TICKS(config::SERVO_MOVE_DELAY_MS));
        s_servo.detach();
    }
    isDrawing = false;
    ::drawing = false;  // Legacy global
}

void penDown() {
    stopMotors();
    setLedColor(50, 50, 50);
    
    if (!isDrawing) {
        s_servo.attach(config::SERVO_PIN);
        s_servo.write(config::SERVO_PEN_UP);
        vTaskDelay(pdMS_TO_TICKS(config::SERVO_MOVE_DELAY_MS));
        s_servo.detach();
    }
    isDrawing = true;
    ::drawing = true;  // Legacy global
}

PenState getPenState() {
    return isDrawing ? PenState::DOWN : PenState::UP;
}

void setServoAngle(int angle) {
    s_servo.attach(config::SERVO_PIN);
    s_servo.write(angle);
    vTaskDelay(pdMS_TO_TICKS(config::SERVO_MOVE_DELAY_MS));
    s_servo.detach();
}

}  // namespace hal
}  // namespace tiny_turtle

void penUp() { tiny_turtle::hal::penUp(); }
void penDown() { tiny_turtle::hal::penDown(); }
