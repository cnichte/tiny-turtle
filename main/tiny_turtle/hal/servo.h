#pragma once
/**
 * @file hal/servo.h
 * @brief Hardware Abstraction Layer für Servo (Stift heben/senken)
 */

#include <cstdint>
#include "../core/types.h"

namespace tiny_turtle
{
    namespace hal
    {

        /**
         * @brief Servo initialisieren
         */
        void initServo();

        /**
         * @brief Stift anheben
         */
        void penUp();

        /**
         * @brief Stift absenken
         */
        void penDown();

        /**
         * @brief Aktuellen Stift-Zustand abfragen
         */
        PenState getPenState();

        /**
         * @brief Servo auf bestimmten Winkel setzen
         * @param angle Winkel in Grad (0-180)
         */
        void setServoAngle(int angle);

    } // namespace hal
} // namespace tiny_turtle

// Legacy-Kompatibilität
void penUp();
void penDown();
