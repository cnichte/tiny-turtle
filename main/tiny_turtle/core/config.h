#pragma once
/**
 * @file config.h
 * @brief Hardware-Konfiguration und Pin-Definitionen für Tiny Turtle
 *
 * Zentrale Konfigurationsdatei für alle Hardware-abhängigen Einstellungen.
 * Hier werden GPIO-Pins, Timing-Parameter und Roboter-Geometrie definiert.
 */

#include <cstdint>

namespace tiny_turtle
{
    namespace config
    {
        //===========================================================================
        // GPIO Pin-Definitionen
        //===========================================================================

        //-----------------------------------------------------------------------
        // GPIO	  Grund	            Beschreibung
        //-----------------------------------------------------------------------
        // 0, 1	  USB	            D- und D+ für USB-Verbindung
        // 9	  Strapping	        Boot-Mode Auswahl (muss HIGH beim Boot)
        // 12, 13 USB Serial/JTAG	Für Debugging/Flash reserviert
        // 14	  Existiert nicht	ESP32-C6 hat keinen GPIO 14
        // 24-30. Existieren nicht	ESP32-C6 hat nur GPIO 0-23
        //-----------------------------------------------------------------------
        // Daraus folgen die ESP32-C6 Pin-Einschränkungen:
        // - GPIO 0, 1: USB D+/D-         - NICHT verwenden!
        // - GPIO 9: Boot Strapping       - NICHT verwenden!
        // - GPIO 12, 13: USB Serial/JTAG - NICHT verwenden!
        // Motor 1: GPIO 10, 11, 14, 15 (save)
        // Motor 2: GPIO 18, 19, 20, 21 (save)
        // Debug-LED: GPIO 8 (onboard)
        //-----------------------------------------------------------------------
        constexpr int A4 = 4;
        constexpr int A5 = 5;
        constexpr int A6 = 6;
        constexpr int A7 = 2;

        // Schrittmotor 1 (links) - ESP32-C6 hat KEINE GPIOs 14, 24-30!
        constexpr int STEP_1A = 10;
        constexpr int STEP_1B = 11;
        constexpr int STEP_1C = 15;
        constexpr int STEP_1D = 22;

        // Schrittmotor 2 (rechts) - Pins 18,19,20,21 (konfliktfrei)
        constexpr int STEP_2A = 18;
        constexpr int STEP_2B = 19;
        constexpr int STEP_2C = 20;
        constexpr int STEP_2D = 21;

        // Peripherie
        constexpr int SERVO_PIN = 4;
        constexpr int SPEAKER_PIN = 3;
        constexpr int NEOPIXEL_PIN = 2;
        constexpr int DEBUG_LED_PIN = 8; // Onboard RGB-LED auf ESP32-C6 DevKit

        // Sensoren
        constexpr int SWITCH_FRONT = A4;
        constexpr int SWITCH_BACK = A5;
        constexpr int PHOTO_SENSOR = A6;

        //===========================================================================
        // Timing-Parameter
        //===========================================================================

        constexpr uint16_t MIN_STEP_DELAY_US = 1000; // Minimale Verzögerung (max. Geschwindigkeit)
        constexpr uint16_t MAX_STEP_DELAY_US = 3000; // Maximale Verzögerung (min. Geschwindigkeit)
        constexpr uint16_t DEFAULT_STEP_DELAY_US = 2000;
        constexpr uint16_t RAMP_VALUE = 5; // Beschleunigungsrate (kleiner = sanfter)

        //===========================================================================
        // Roboter-Geometrie
        //===========================================================================

        constexpr float PI = 3.14159265359f;

        // Schrittmotor-Eigenschaften (28BYJ-48)
        constexpr int STEPS_PER_MOTOR_ROTATION = 64; // Interne Schritte
        constexpr int GEAR_RATIO = 64; // Getriebeübersetzung
        constexpr int STEPS_PER_ROTATION = STEPS_PER_MOTOR_ROTATION * GEAR_RATIO; // = 4096

        // Rad-Geometrie (in mm)
        constexpr float WHEEL_DIAMETER = 24.05f;
        constexpr float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * PI;
        constexpr float WHEEL_DISTANCE = 33.5f; // Abstand zwischen den Rädern

        // Berechnete Werte
        constexpr float STEPS_PER_MM = static_cast<float>(STEPS_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
        constexpr float STEPS_PER_360_ROTATION = WHEEL_DISTANCE * PI * STEPS_PER_MM;

        //===========================================================================
        // Servo-Konfiguration (Stift heben/senken)
        //===========================================================================

        constexpr int SERVO_PEN_DOWN = 0;
        constexpr int SERVO_PEN_UP = 180;
        constexpr int SERVO_MOVE_DELAY_MS = 400; // Wartezeit für Servo-Bewegung

        //===========================================================================
        // Sensor-Konfiguration
        //===========================================================================

        constexpr float SENSOR_OFFSET_MM = 16.0f; // Abstand Sensor zu Roboter-Zentrum
        constexpr int SENSOR_THRESHOLD = 512;     // ADC-Schwellwert für Dunkel-Erkennung

    } // namespace config
} // namespace tiny_turtle
