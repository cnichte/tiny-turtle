#pragma once
/**
 * @file motion/spiral.h
 * @brief Spiral-Bewegungen und Kreise
 */

namespace tiny_turtle {
namespace motion {

/**
 * @brief Spirale zeichnen
 * @param initialRadiusMM Startradius
 * @param finalRadiusMM Endradius
 * @param revolutions Anzahl Umdrehungen
 * @param clockwise Richtung
 * @param stopAtSwitch Bei Bumper stoppen
 * @param stopAtSensor Bei Sensor stoppen
 * @param togglePenAtSensor Stift bei Sensor umschalten
 * @return true wenn erfolgreich
 */
bool spiral(double initialRadiusMM, double finalRadiusMM, double revolutions = 1.0,
            bool clockwise = true, bool stopAtSwitch = true, 
            bool stopAtSensor = false, bool togglePenAtSensor = false);

/**
 * @brief Kreis zeichnen
 * @param radius Radius in mm
 * @param clockwise Im Uhrzeigersinn?
 */
void circle(float radius, bool clockwise = false);

/**
 * @brief Kreisbogen zeichnen
 * @param radius Radius in mm
 * @param degrees Winkel in Grad
 * @param clockwise Im Uhrzeigersinn?
 */
void arc(float radius, float degrees, bool clockwise = false);

}  // namespace motion
}  // namespace tiny_turtle

// Legacy-Kompatibilität
bool spiral(double initialRadiusMM, double finalRadiusMM, double revolutions = 1.0,
            bool clockwise = true, bool stopAtSwitch = true, 
            bool stopAtSensor = false, bool togglePenAtSensor = false);
