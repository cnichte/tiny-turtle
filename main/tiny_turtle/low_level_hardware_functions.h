#pragma once

#include <cstdint>

void switchStepper(uint8_t stepper, int direction);
void stopSteppers();
void penUp();
void penDown();
bool checkBumper();
bool checkSensor();
