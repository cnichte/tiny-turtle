/**
 * @file hal/led.cpp
 * @brief Implementierung der LED-Steuerung
 */

#include "led.h"
#include "../core/config.h"
#include "neopixel.h"

namespace tiny_turtle {
namespace hal {

// NeoPixel-Instanz
static NeoPixel s_pixels(1, config::NEOPIXEL_PIN);

void initLed() {
    s_pixels.begin();
    s_pixels.show();
}

void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
    s_pixels.setPixelColor(0, s_pixels.Color(r, g, b));
    s_pixels.show();
}

void ledOff() {
    setLedColor(0, 0, 0);
}

// Globale Instanz für Legacy-Zugriff
NeoPixel& getPixels() {
    return s_pixels;
}

}  // namespace hal
}  // namespace tiny_turtle

void led(uint8_t r, uint8_t g, uint8_t b) {
    tiny_turtle::hal::setLedColor(r, g, b);
}
