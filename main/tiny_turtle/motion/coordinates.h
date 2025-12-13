#pragma once
/**
 * @file motion/coordinates.h
 * @brief Koordinatenbasierte Bewegung
 */

#include "../core/types.h"
#include <cstdint>

namespace tiny_turtle
{
    namespace motion
    {

        /**
         * @brief Position zurücksetzen
         */
        void resetPosition();

        /**
         * @brief Position manuell setzen
         */
        void setPosition(float x, float y, float heading);

        /**
         * @brief Aktuelle Position abfragen
         */
        Point2D getPosition();

        /**
         * @brief Aktuelle Ausrichtung abfragen (in Grad)
         */
        float getHeading();

        /**
         * @brief Zu Koordinate fahren
         * @param x Ziel-X in mm
         * @param y Ziel-Y in mm
         * @param penDown Mit Stift zeichnen?
         */
        void goTo(float x, float y, bool penDown = false);

        /**
         * @brief Koordinaten-Array zeichnen
         * @param coords X,Y Paare
         * @param count Anzahl der Werte (nicht Punkte!)
         * @param scale Skalierungsfaktor
         */
        void drawCoordinates(const uint8_t *coords, int count, float scale = 1.0f);

        /**
         * @brief Punkte-Array zeichnen
         * @param points Array von Punkten
         * @param count Anzahl Punkte
         * @param closed Zurück zum Start?
         */
        void drawShape(const Point2D *points, int count, bool closed = true);

        /**
         * @brief Koordinaten-Zeichnung ausführen (Legacy)
         * @param scale Skalierungsfaktor
         */
        void drawCoordinates(float scale);

    } // namespace motion
} // namespace tiny_turtle

// Legacy-Kompatibilität
void goTo(float x, float y, bool penDown = false);
void drawCoordinates(const uint8_t *coords, int count, float scale = 1.0f);
void drawCoordinates(float scale);
