#pragma once
/**
 * @file config.h
 * @brief Hardware-Konfiguration und Pin-Definitionen für Tiny Turtle
 * 
 * Zentrale Konfigurationsdatei für alle Hardware-abhängigen Einstellungen.
 * Hier werden GPIO-Pins, Timing-Parameter und Roboter-Geometrie definiert.
 */

#include <cstdint>

namespace tiny_turtle {
namespace config {

//===========================================================================
// GPIO Pin-Definitionen
//===========================================================================

// Analog-Pin Mappings (Arduino -> ESP32)
constexpr int A4 = 4;
constexpr int A5 = 5;
constexpr int A6 = 6;
constexpr int A7 = 2;

// Schrittmotor 1 (links)
constexpr int STEP_1A = 9;
constexpr int STEP_1B = 10;
constexpr int STEP_1C = 11;
constexpr int STEP_1D = 12;

// Schrittmotor 2 (rechts)
constexpr int STEP_2A = 8;
constexpr int STEP_2B = 7;
constexpr int STEP_2C = 6;
constexpr int STEP_2D = 5;

// Peripherie
constexpr int SERVO_PIN = 4;
constexpr int SPEAKER_PIN = 3;
constexpr int NEOPIXEL_PIN = 2;

// Sensoren
constexpr int SWITCH_FRONT = A4;
constexpr int SWITCH_BACK = A5;
constexpr int PHOTO_SENSOR = A6;

//===========================================================================
// Timing-Parameter
//===========================================================================

constexpr uint16_t MIN_STEP_DELAY_US = 1000;    // Minimale Verzögerung (max. Geschwindigkeit)
constexpr uint16_t MAX_STEP_DELAY_US = 3000;    // Maximale Verzögerung (min. Geschwindigkeit)
constexpr uint16_t DEFAULT_STEP_DELAY_US = 2000;
constexpr uint16_t RAMP_VALUE = 5;              // Beschleunigungsrate (kleiner = sanfter)

//===========================================================================
// Roboter-Geometrie
//===========================================================================

constexpr float PI = 3.14159265359f;

// Schrittmotor-Eigenschaften (28BYJ-48)
constexpr int STEPS_PER_MOTOR_ROTATION = 64;    // Interne Schritte
constexpr int GEAR_RATIO = 64;                   // Getriebeübersetzung
constexpr int STEPS_PER_ROTATION = STEPS_PER_MOTOR_ROTATION * GEAR_RATIO;  // = 4096

// Rad-Geometrie (in mm)
constexpr float WHEEL_DIAMETER = 24.05f;
constexpr float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * PI;
constexpr float WHEEL_DISTANCE = 33.5f;         // Abstand zwischen den Rädern

// Berechnete Werte
constexpr float STEPS_PER_MM = static_cast<float>(STEPS_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
constexpr float STEPS_PER_360_ROTATION = WHEEL_DISTANCE * PI * STEPS_PER_MM;

//===========================================================================
// Servo-Konfiguration (Stift heben/senken)
//===========================================================================

constexpr int SERVO_PEN_DOWN = 0;
constexpr int SERVO_PEN_UP = 180;
constexpr int SERVO_MOVE_DELAY_MS = 400;        // Wartezeit für Servo-Bewegung

//===========================================================================
// Sensor-Konfiguration
//===========================================================================

constexpr float SENSOR_OFFSET_MM = 16.0f;       // Abstand Sensor zu Roboter-Zentrum
constexpr int SENSOR_THRESHOLD = 512;            // ADC-Schwellwert für Dunkel-Erkennung

}  // namespace config
}  // namespace tiny_turtle

// Legacy-Kompatibilität: Alte Konstantennamen beibehalten
// TODO: Schrittweise durch namespace-Versionen ersetzen
constexpr int A4 = tiny_turtle::config::A4;
constexpr int A5 = tiny_turtle::config::A5;
constexpr int A6 = tiny_turtle::config::A6;
constexpr int A7 = tiny_turtle::config::A7;

constexpr int STEP_1A = tiny_turtle::config::STEP_1A;
constexpr int STEP_1B = tiny_turtle::config::STEP_1B;
constexpr int STEP_1C = tiny_turtle::config::STEP_1C;
constexpr int STEP_1D = tiny_turtle::config::STEP_1D;
constexpr int STEP_2A = tiny_turtle::config::STEP_2A;
constexpr int STEP_2B = tiny_turtle::config::STEP_2B;
constexpr int STEP_2C = tiny_turtle::config::STEP_2C;
constexpr int STEP_2D = tiny_turtle::config::STEP_2D;

constexpr int SERVO = tiny_turtle::config::SERVO_PIN;
constexpr int SPEAKER = tiny_turtle::config::SPEAKER_PIN;
constexpr int NEOPIXEL = tiny_turtle::config::NEOPIXEL_PIN;
constexpr int SWITCH_F = tiny_turtle::config::SWITCH_FRONT;
constexpr int SWITCH_B = tiny_turtle::config::SWITCH_BACK;
constexpr int SENSOR = tiny_turtle::config::PHOTO_SENSOR;

constexpr float PI = tiny_turtle::config::PI;
constexpr uint16_t minDelay = tiny_turtle::config::MIN_STEP_DELAY_US;
constexpr uint16_t maxDelay = tiny_turtle::config::MAX_STEP_DELAY_US;
constexpr uint16_t rampValue = tiny_turtle::config::RAMP_VALUE;
constexpr int stepsPerRotation = tiny_turtle::config::STEPS_PER_ROTATION;
constexpr float wheelCircumference = tiny_turtle::config::WHEEL_CIRCUMFERENCE;
constexpr float stepsPerMM = tiny_turtle::config::STEPS_PER_MM;
constexpr float wheelDistance = tiny_turtle::config::WHEEL_DISTANCE;
constexpr float stepsPer360Rotation = tiny_turtle::config::STEPS_PER_360_ROTATION;
constexpr int servoPenDown = tiny_turtle::config::SERVO_PEN_DOWN;
constexpr int servoPenUp = tiny_turtle::config::SERVO_PEN_UP;
constexpr float sensorOffset = tiny_turtle::config::SENSOR_OFFSET_MM;
