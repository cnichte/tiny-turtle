/**
 * @file motion/coordinates.cpp
 * @brief Koordinatenbasierte Bewegung
 */

#include "coordinates.h"
#include "motion.h"
#include "../hal/servo.h"
#include "../math/trigonometry.h"
#include "../core/globals.h"
#include <cmath>

namespace tiny_turtle {
namespace motion {

// Aktuelle Position des Roboters
static float currentX = 0.0f;
static float currentY = 0.0f;
static float currentHeading = 0.0f;  // 0 = nach vorne (positive Y)

void resetPosition() {
    currentX = 0.0f;
    currentY = 0.0f;
    currentHeading = 0.0f;
}

void setPosition(float x, float y, float heading) {
    currentX = x;
    currentY = y;
    currentHeading = heading;
}

Point2D getPosition() {
    return {currentX, currentY};
}

float getHeading() {
    return currentHeading;
}

void goTo(float targetX, float targetY, bool penDown) {
    // Differenz berechnen
    float dx = targetX - currentX;
    float dy = targetY - currentY;
    
    // Distanz und Winkel berechnen
    float distance = sqrt(dx * dx + dy * dy);
    float targetAngle = atan2(dx, dy) * 180.0f / M_PI;  // atan2(x,y) für Heading-Konvention
    
    // Winkel zum Ziel drehen
    float turnAngle = targetAngle - currentHeading;
    
    // Winkel normalisieren auf -180 bis 180
    while (turnAngle > 180.0f) turnAngle -= 360.0f;
    while (turnAngle < -180.0f) turnAngle += 360.0f;
    
    // Drehen wenn nötig
    if (abs(turnAngle) > 1.0f) {
        turn(turnAngle);
        currentHeading = targetAngle;
    }
    
    // Zum Ziel fahren
    if (penDown) {
        hal::penDown();
    }
    
    forward(distance);
    
    if (penDown) {
        hal::penUp();
    }
    
    // Position aktualisieren
    currentX = targetX;
    currentY = targetY;
}

void drawCoordinates(const uint8_t* coords, int count, float scale) {
    // Koordinaten sind als X,Y Paare kodiert
    for (int i = 0; i < count; i += 2) {
        float x = coords[i] * scale;
        float y = coords[i + 1] * scale;
        
        if (i == 0) {
            // Zum ersten Punkt ohne Zeichnen
            goTo(x, y, false);
            hal::penDown();
        } else {
            goTo(x, y, true);
        }
    }
    hal::penUp();
}

void drawShape(const Point2D* points, int count, bool closed) {
    if (count < 2) return;
    
    // Zum ersten Punkt
    goTo(points[0].x, points[0].y, false);
    hal::penDown();
    
    // Alle weiteren Punkte
    for (int i = 1; i < count; i++) {
        goTo(points[i].x, points[i].y, true);
    }
    
    // Zurück zum Start wenn geschlossen
    if (closed) {
        goTo(points[0].x, points[0].y, true);
    }
    
    hal::penUp();
}

}  // namespace motion
}  // namespace tiny_turtle

// Legacy-Kompatibilität
void goTo(float x, float y, bool penDown) {
    tiny_turtle::motion::goTo(x, y, penDown);
}

void drawCoordinates(const uint8_t* coords, int count, float scale) {
    tiny_turtle::motion::drawCoordinates(coords, count, scale);
}
