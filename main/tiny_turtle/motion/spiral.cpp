/**
 * @file motion/spiral.cpp
 * @brief Spiralbewegungen
 */

#include "spiral.h"
#include "motion.h"
#include "../hal/servo.h"
#include "../core/config.h"
#include <cmath>
#include <cstdlib>

namespace tiny_turtle {
namespace motion {

using namespace config;

void spiral(float startRadius, float endRadius, float revolutions, bool penDown) {
    if (penDown) {
        hal::penDown();
    }
    
    const int stepsPerRev = 36;  // 10° Schritte
    int totalSteps = static_cast<int>(revolutions * stepsPerRev);
    float radiusStep = (endRadius - startRadius) / totalSteps;
    float currentRadius = startRadius;
    
    for (int i = 0; i < totalSteps; i++) {
        float arcLength = (2.0f * PI * currentRadius) / stepsPerRev;
        forward(arcLength);
        turn(360.0f / stepsPerRev);
        currentRadius += radiusStep;
    }
    
    if (penDown) {
        hal::penUp();
    }
}

void spiralIn(float radius, float revolutions) {
    spiral(radius, 0, revolutions, true);
}

void spiralOut(float radius, float revolutions) {
    spiral(0, radius, revolutions, true);
}

void circle(float radius, bool clockwise) {
    const int steps = 36;
    float arcLength = (2.0f * PI * radius) / steps;
    float angleStep = clockwise ? -10.0f : 10.0f;
    
    for (int i = 0; i < steps; i++) {
        forward(arcLength);
        turn(angleStep);
    }
}

void arc(float radius, float degrees, bool clockwise) {
    int steps = static_cast<int>(fabs(degrees) / 10.0f);
    if (steps < 1) steps = 1;
    
    float arcLength = (2.0f * PI * radius * fabs(degrees) / 360.0f) / steps;
    float angleStep = degrees / steps;
    if (clockwise) angleStep = -angleStep;
    
    for (int i = 0; i < steps; i++) {
        forward(arcLength);
        turn(angleStep);
    }
}

}  // namespace motion
}  // namespace tiny_turtle

// Legacy-Kompatibilität
void spiral(float startRadius, float endRadius, float revolutions, bool penDown) {
    tiny_turtle::motion::spiral(startRadius, endRadius, revolutions, penDown);
}

void spiralIn(float radius, float revolutions) {
    tiny_turtle::motion::spiralIn(radius, revolutions);
}

void spiralOut(float radius, float revolutions) {
    tiny_turtle::motion::spiralOut(radius, revolutions);
}

void circle(float radius, bool clockwise) {
    tiny_turtle::motion::circle(radius, clockwise);
}

void arc(float radius, float degrees, bool clockwise) {
    tiny_turtle::motion::arc(radius, degrees, clockwise);
}
