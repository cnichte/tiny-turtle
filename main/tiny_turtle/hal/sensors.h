#pragma once
/**
 * @file hal/sensors.h
 * @brief Hardware Abstraction Layer für Sensoren (Bumper, Fotosensor)
 */

#include <cstdint>
#include "../core/types.h"

namespace tiny_turtle
{
    namespace hal
    {
        /**
         * @brief Sensoren initialisieren (GPIO konfigurieren)
         */
        void initSensors();

        /**
         * @brief Bumper-Schalter abfragen
         * @return BumperState mit front/back Zuständen
         */
        BumperState readBumpers();

        /**
         * @brief Prüfen ob irgendein Bumper gedrückt ist
         */
        bool isBumperPressed();

        /**
         * @brief Fotosensor lesen
         * @return ADC-Wert (0-4095)
         */
        int readPhotoSensor();

        /**
         * @brief Prüfen ob Fotosensor etwas Dunkles erkennt
         */
        bool isDarkDetected();

    } // namespace hal
} // namespace tiny_turtle

// Legacy-Kompatibilität
bool checkBumper();
bool checkSensor();
