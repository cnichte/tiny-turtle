#pragma once

#include <cstdint>

/**
 * Timer-basierte Schrittmotor-Steuerung für ESP32
 * Verwendet GPTimer mit Interrupt für präzises Timing
 * 
 * Vorteile gegenüber Software-Delays:
 * - Minimale CPU-Auslastung
 * - Hardware-präzises Timing
 * - Einfache Geschwindigkeitsanpassung
 * - Sanfte Beschleunigungsrampen möglich
 */

// Motor-Befehle
enum class MotorCommand {
    STOP,           // Motoren stoppen
    FORWARD,        // Beide Motoren vorwärts
    BACKWARD,       // Beide Motoren rückwärts
    SPIN_CW,        // Drehung auf der Stelle (Uhrzeigersinn)
    SPIN_CCW,       // Drehung auf der Stelle (gegen Uhrzeigersinn)
};

// Timer-basierte Motor-Steuerung initialisieren
void stepper_timer_init(void);

// Timer starten/stoppen
void stepper_timer_start(void);
void stepper_timer_stop(void);

// Motor-Befehl setzen
void stepper_set_command(MotorCommand cmd);

// Geschwindigkeit setzen (Step-Intervall in Mikrosekunden)
// Kleiner = schneller, Bereich ca. 1000-5000 µs
void stepper_set_speed(uint32_t step_interval_us);

// Aktuelle Geschwindigkeit abfragen
uint32_t stepper_get_speed(void);

// Anzahl der ausgeführten Schritte (für Positionierung)
int32_t stepper_get_step_count(void);
void stepper_reset_step_count(void);

// Prüfen ob Motoren laufen
bool stepper_is_running(void);

// Rampen-Steuerung
// Aktiviert sanfte Beschleunigung/Verzögerung
// ramp_steps: Anzahl Schritte für vollständige Rampe (0 = deaktiviert)
// Beispiel: ramp_steps=100 bedeutet 100 Schritte von Start- bis Zielgeschwindigkeit
void stepper_set_ramp(uint32_t ramp_steps);

// Geschwindigkeit mit Rampe setzen
// Fährt sanft von aktueller zu neuer Geschwindigkeit
void stepper_set_target_speed(uint32_t target_speed_us);

// Prüfen ob Rampe aktiv (noch beschleunigt/verzögert)
bool stepper_is_ramping(void);

// Sanft stoppen (mit Verzögerungsrampe)
void stepper_smooth_stop(void);
