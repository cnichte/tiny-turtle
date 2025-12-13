#pragma once
/**
 * @file motion/motion.h
 * @brief Bewegungsfunktionen für den Roboter
 */

#include <cstdint>

namespace tiny_turtle {
namespace motion {

/**
 * @brief Vorwärts bewegen
 * @param distanceMm Distanz in Millimetern
 */
void forward(float distanceMm);

/**
 * @brief Rückwärts bewegen
 * @param distanceMm Distanz in Millimetern
 */
void backward(float distanceMm);

/**
 * @brief Bewegen mit Kollisionserkennung
 * @param distanceMm Distanz in Millimetern
 * @param bounceAtObstacle Bei Kollision ausweichen
 * @return true wenn erfolgreich
 */
bool move(float distanceMm, bool bounceAtObstacle = true);

/**
 * @brief Auf der Stelle drehen
 * @param degrees Winkel in Grad
 * @param clockwise true = Uhrzeigersinn
 */
void turn(float degrees, int turningDirection = 1);

/**
 * @brief Optimierte Drehung (max 90°)
 * @param angle Zielwinkel
 */
void smartTurn(float angle);

/**
 * @brief Bei Kollision ausweichen
 */
void bounce();

}  // namespace motion
}  // namespace tiny_turtle

// Legacy-Kompatibilität
void forward(float distance);
void back(float distance);
bool move(float distance, bool bounceAtObstacle = true);
void turn(float degrees, int turningDirection = 1);
void smartTurn(float angle);
void bounce();
