#include "globals.h"

namespace tiny_turtle
{
    // Schrittmotor-Zähler
    uint8_t stepCount1 = 0;
    uint8_t stepCount2 = 0;

    // Bewegungs-Zustand
    long delayValue = 2000;
    int direction = 1;
    bool isDrawing = false;

} // namespace tiny_turtle

// Legacy-Variablen (separate Kopien für Rückwärtskompatibilität)
uint8_t stepCount1 = 0;
uint8_t stepCount2 = 0;
long delayValue = 2000;
int direction = 1;
bool drawing = false;
