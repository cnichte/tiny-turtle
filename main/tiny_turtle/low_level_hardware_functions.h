#pragma once

#include <cstdint>
#include "esp_attr.h"  // für IRAM_ATTR

// ISR-safe Funktionen (können aus Interrupts aufgerufen werden)
void IRAM_ATTR switchStepper(uint8_t stepper, int direction);
void IRAM_ATTR stopSteppers();

// Normale Funktionen
void penUp();
void penDown();
bool checkBumper();
bool checkSensor();
