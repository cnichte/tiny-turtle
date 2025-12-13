/**
 * @file math/trigonometry.cpp
 * @brief Trigonometrische Hilfsfunktionen
 */

#include "trigonometry.h"
#include <cmath>

namespace tiny_turtle {
namespace math {

float calculateSignedAngleABC(float Ax, float Ay, float Bx, float By, float Cx, float Cy) {
    float BAx = Ax - Bx, BAy = Ay - By;
    float BCx = Cx - Bx, BCy = Cy - By;
    
    float dotProduct = BAx * BCx + BAy * BCy;
    float crossProduct = BAx * BCy - BAy * BCx;
    
    float angleRad = atan2(crossProduct, dotProduct);
    return angleRad * (180.0f / M_PI);
}

float calculateDistanceBC(float Bx, float By, float Cx, float Cy) {
    return sqrt((Cx - Bx) * (Cx - Bx) + (Cy - By) * (Cy - By));
}

int sign(float x) {
    return (x > 0) - (x < 0);
}

}  // namespace math
}  // namespace tiny_turtle

// Legacy-Kompatibilität
float calculateSignedAngleABC(float Ax, float Ay, float Bx, float By, float Cx, float Cy) {
    return tiny_turtle::math::calculateSignedAngleABC(Ax, Ay, Bx, By, Cx, Cy);
}

float calculateDistanceBC(float Bx, float By, float Cx, float Cy) {
    return tiny_turtle::math::calculateDistanceBC(Bx, By, Cx, Cy);
}

int sign(float x) {
    return tiny_turtle::math::sign(x);
}
