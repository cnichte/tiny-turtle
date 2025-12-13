#pragma once
/**
 * @file hal/stepper.h
 * @brief Hardware Abstraction Layer für Schrittmotoren
 *
 * Bietet sowohl Low-Level ISR-sichere Funktionen als auch
 * die Timer-basierte High-Level-Steuerung.
 */

#include <cstdint>
#include "../core/types.h"
#include "esp_attr.h"

namespace tiny_turtle
{
    namespace hal
    {

        //===========================================================================
        // Low-Level Stepper-Funktionen (ISR-safe)
        //===========================================================================

        /**
         * @brief Einen Schritt auf einem Motor ausführen
         * @param stepper Motor-Nummer (1 oder 2)
         * @param direction Richtung (-1, 0, 1)
         * @note ISR-safe, kann aus Interrupts aufgerufen werden
         */
        void stepMotor(uint8_t stepper, int direction);

        /**
         * @brief Alle Motor-Spulen stromlos schalten
         * @note ISR-safe, spart Energie wenn Motoren nicht benötigt werden
         */
        void stopMotors();

        //===========================================================================
        // Timer-basierte Motor-Steuerung
        //===========================================================================

        /**
         * @brief GPTimer für Motorsteuerung initialisieren
         */
        void initStepperTimer();

        /**
         * @brief Timer starten (Motoren beginnen zu laufen)
         */
        void startStepperTimer();

        /**
         * @brief Timer stoppen
         */
        void stopStepperTimer();

        /**
         * @brief Motor-Befehl setzen
         * @param cmd Gewünschte Bewegung (FORWARD, BACKWARD, SPIN_CW, etc.)
         */
        void setMotorCommand(MotorCommand cmd);

        /**
         * @brief Schrittgeschwindigkeit setzen
         * @param stepIntervalUs Intervall zwischen Schritten in Mikrosekunden
         *                       Kleiner = schneller (min: 500, max: 10000)
         */
        void setStepSpeed(uint32_t stepIntervalUs);

        /**
         * @brief Aktuelle Geschwindigkeit abfragen
         */
        uint32_t getStepSpeed();

        /**
         * @brief Schrittzähler abfragen
         */
        int32_t getStepCount();

        /**
         * @brief Schrittzähler zurücksetzen
         */
        void resetStepCount();

        /**
         * @brief Prüfen ob Motoren aktiv sind
         */
        bool isMotorRunning();

        //===========================================================================
        // Rampen-Steuerung
        //===========================================================================

        /**
         * @brief Beschleunigungs-/Verzögerungsrampe aktivieren
         * @param rampSteps Anzahl Schritte für volle Rampe (0 = deaktiviert)
         */
        void setRamp(uint32_t rampSteps);

        /**
         * @brief Zielgeschwindigkeit mit Rampe anfahren
         * @param targetSpeedUs Ziel-Intervall in Mikrosekunden
         */
        void setTargetSpeed(uint32_t targetSpeedUs);

        /**
         * @brief Prüfen ob Rampe aktiv ist
         */
        bool isRamping();

        /**
         * @brief Sanft stoppen (mit Verzögerungsrampe)
         */
        void smoothStop();

    } // namespace hal
} // namespace tiny_turtle

// Legacy-Kompatibilität (IRAM_ATTR nur in Definition, nicht Deklaration)
void switchStepper(uint8_t stepper, int direction);
void stopSteppers();

void stepper_timer_init();
void stepper_timer_start();
void stepper_timer_stop();
void stepper_set_command(MotorCommand cmd);
void stepper_set_speed(uint32_t step_interval_us);
uint32_t stepper_get_speed();
int32_t stepper_get_step_count();
void stepper_reset_step_count();
bool stepper_is_running();
void stepper_set_ramp(uint32_t steps);
void stepper_set_target_speed(uint32_t target_speed_us);
bool stepper_is_ramping();
void stepper_smooth_stop();
