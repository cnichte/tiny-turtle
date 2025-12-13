#pragma once
/**
 * @file math/trigonometry.h
 * @brief Trigonometrische Hilfsfunktionen
 */

namespace tiny_turtle
{
    namespace math
    {
        /**
         * @brief Winkel zwischen drei Punkten berechnen
         * @param Ax,Ay Punkt A
         * @param Bx,By Punkt B (Scheitelpunkt)
         * @param Cx,Cy Punkt C
         * @return Winkel in Grad (kann negativ sein)
         */
        float calculateSignedAngleABC(float Ax, float Ay, float Bx, float By, float Cx, float Cy);

        /**
         * @brief Distanz zwischen zwei Punkten
         */
        float calculateDistanceBC(float Bx, float By, float Cx, float Cy);

        /**
         * @brief Vorzeichen einer Zahl
         * @return -1, 0 oder 1
         */
        int sign(float x);

    } // namespace math
} // namespace tiny_turtle

// Legacy-Kompatibilität
float calculateSignedAngleABC(float Ax, float Ay, float Bx, float By, float Cx, float Cy);
float calculateDistanceBC(float Bx, float By, float Cx, float Cy);
int sign(float x);
