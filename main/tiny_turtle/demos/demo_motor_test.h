#pragma once
/**
 * @file demo_motor_test.h
 * @brief Demo: GPTimer Motor-Test mit Beschleunigung/Verzögerung (Rampen)
 */

namespace tiny_turtle
{
    namespace demos
    {
        /**
         * @brief Testet die Motoren mit Rampen-Beschleunigung
         *
         * Führt folgende Tests durch:
         * 1. Vorwärts fahren mit Beschleunigung
         * 2. Volle Geschwindigkeit halten
         * 3. Sanftes Stoppen
         * 4. Drehung CW mit Rampe
         * 5. Abbremsen und stoppen
         */
        void runMotorTest();

        /**
         * @brief Einfacher Dauerrotationstest (beide Räder vorwärts)
         * Nützlich zum Testen ob die Motoren grundsätzlich funktionieren.
         */
        void runSimpleRotationTest();

    } // namespace demos
} // namespace tiny_turtle
