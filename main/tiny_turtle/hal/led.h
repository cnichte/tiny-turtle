#pragma once
/**
 * @file hal/led.h
 * @brief Hardware Abstraction Layer für LED (NeoPixel)
 */

#include <cstdint>

namespace tiny_turtle
{
    namespace hal
    {

        /**
         * @brief LED initialisieren
         */
        void initLed();

        /**
         * @brief LED-Farbe setzen
         * @param r Rot (0-255)
         * @param g Grün (0-255)
         * @param b Blau (0-255)
         */
        void setLedColor(uint8_t r, uint8_t g, uint8_t b);

        /**
         * @brief LED ausschalten
         */
        void ledOff();

    } // namespace hal
} // namespace tiny_turtle

// Legacy-Kompatibilität
void led(uint8_t r, uint8_t g, uint8_t b);
