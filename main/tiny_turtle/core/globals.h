#pragma once
/**
 * @file globals.h
 * @brief Globale Zustandsvariablen für Tiny Turtle
 * 
 * Diese Variablen werden für den aktuellen Zustand des Roboters verwendet.
 * Langfristig sollten diese in eine State-Klasse gekapselt werden.
 */

#include <cstdint>

namespace tiny_turtle {

//===========================================================================
// Roboter-Zustand
//===========================================================================

// Schrittmotor-Zähler (aktuelle Phase im Half-Step-Muster)
extern uint8_t stepCount1;
extern uint8_t stepCount2;

// Bewegungs-Zustand
extern long delayValue;     // Aktuelle Schritt-Verzögerung
extern int direction;       // Aktuelle Bewegungsrichtung (1 = vorwärts, -1 = rückwärts)
extern bool isDrawing;      // Stift-Zustand (true = unten/zeichnet) - renamed to avoid namespace conflict

}  // namespace tiny_turtle

// Legacy-Kompatibilität: Globale Variablen exportieren
extern uint8_t stepCount1;
extern uint8_t stepCount2;
extern long delayValue;
extern int direction;
extern bool drawing;  // Legacy name
