#pragma once
/**
 * @file tiny_turtle.h
 * @brief Tiny Turtle Robot - Öffentliche API
 * 
 * Diese Datei ist der zentrale Einstiegspunkt für die Tiny Turtle Bibliothek.
 * Inkludieren Sie diese Datei für den vollen Zugriff auf alle Funktionen.
 * 
 * @author Carsten Nichte
 * @version 2.0
 */

#include <cstdint>
#include <string>

// ============================================================================
// Core Module
// ============================================================================
#include "core/config.h"     // Hardware-Konfiguration und Konstanten
#include "core/types.h"      // Gemeinsame Typen und Enums
#include "core/globals.h"    // Globale Zustandsvariablen

// ============================================================================
// HAL (Hardware Abstraction Layer)
// ============================================================================
#include "hal/gpio_hal.h"    // GPIO-Wrapper (pinMode, digitalWrite, delay, etc.)
#include "hal/neopixel.h"    // NeoPixel LED-Streifen
#include "hal/stepper.h"     // Schrittmotor-Steuerung mit GPTimer
#include "hal/servo.h"       // Servo für Stift (Pen Up/Down)
#include "hal/sensors.h"     // Bumper und Foto-Sensor
#include "hal/audio.h"       // Speaker/Piezo
#include "hal/led.h"         // NeoPixel LED

// ============================================================================
// Motion Module
// ============================================================================
#include "motion/motion.h"       // Grundlegende Bewegungen (forward, turn, etc.)
#include "motion/spiral.h"       // Spiralen und Kreise
#include "motion/coordinates.h"  // Koordinatenbasierte Bewegung

// ============================================================================
// Drawing Module
// ============================================================================
#include "drawing/text.h"    // Text-Zeichnung
#include "drawing/fonts.h"   // Font-Daten

// ============================================================================
// Math Module
// ============================================================================
#include "math/trigonometry.h"  // Trigonometrische Hilfsfunktionen

// ============================================================================
// Haupt-Namespace
// ============================================================================
namespace tiny_turtle {

/**
 * @brief Tiny Turtle System initialisieren
 */
void init();

/**
 * @brief System stoppen
 */
void shutdown();

/**
 * @brief Hauptschleife
 */
void loop();

/**
 * @brief Version der Bibliothek
 */
constexpr const char* VERSION = "2.0.0";

}  // namespace tiny_turtle

// ============================================================================
// Initialisierung
// ============================================================================
void turtleInit();
void turtleLoop();
