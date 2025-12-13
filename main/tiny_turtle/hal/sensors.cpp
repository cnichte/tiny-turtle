/**
 * @file hal/sensors.cpp
 * @brief Implementierung der Sensor-Abfragen
 */

#include "sensors.h"
#include "../core/config.h"
#include "gpio_hal.h"

namespace tiny_turtle
{
    namespace hal
    {

        void initSensors()
        {
            pinMode(config::SWITCH_FRONT, INPUT_PULLUP);
            pinMode(config::SWITCH_BACK, INPUT_PULLUP);
            // Foto-Sensor wird über ADC gelesen, keine Init nötig
        }

        BumperState readBumpers()
        {
            return {
                .front = !digitalRead(config::SWITCH_FRONT),
                .back = !digitalRead(config::SWITCH_BACK)};
        }

        bool isBumperPressed()
        {
            return !digitalRead(config::SWITCH_FRONT) || !digitalRead(config::SWITCH_BACK);
        }

        int readPhotoSensor()
        {
            return analogRead(config::PHOTO_SENSOR);
        }

        bool isDarkDetected()
        {
            return analogRead(config::PHOTO_SENSOR) < config::SENSOR_THRESHOLD;
        }

    } // namespace hal
} // namespace tiny_turtle

// Legacy-Kompatibilität
bool checkBumper() { return tiny_turtle::hal::isBumperPressed(); }
bool checkSensor() { return tiny_turtle::hal::isDarkDetected(); }
