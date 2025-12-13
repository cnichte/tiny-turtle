#pragma once

#include <cstdint>
#include <string>

#include "arduino_compat.h"
#include "constants.h"
#include "neopixel_compat.h"
#include "low_level_hardware_functions.h"  // switchStepper, stopSteppers etc.

// Shared globals (kept to minimize changes from original sketch).
extern NeoPixel pixels;
extern Servo myservo;
extern uint8_t stepCount1;
extern uint8_t stepCount2;
extern long delayValue;
extern int direction;
extern bool drawing;

// Light/sound helpers
void led(uint8_t r, uint8_t g, uint8_t b);
void triTone();

// Motion helpers sind in low_level_hardware_functions.h deklariert:
// - switchStepper(), stopSteppers() - ISR-safe mit IRAM_ATTR
// - penUp(), penDown(), checkBumper(), checkSensor()

// Higher-level motion
bool move(float distance, bool bounceAtObstacle = true);
void bounce();
void forward(float distance);
void back(float distance);
void turn(float degrees, int turningDirection = 1);
void smartTurn(float angle);
void drawCoordinates(float scale);
bool spiral(double initialRadiusMM, double finalRadiusMM, double revolutions = 1.0, bool clockwise = true, bool stopAtSwitch = true, bool stopAtSensor = false, bool togglePenAtSensor = false);

// Text plotting
void plotText(const std::string& str, int scale);
void plotChar(uint8_t character, float scale);
int ASCIItoIndex(uint8_t c);

// Trigonometry helpers
float calculateSignedAngleABC(float Ax, float Ay, float Bx, float By, float Cx, float Cy);
float calculateDistanceBC(float Bx, float By, float Cx, float Cy);
int sign(float x);
