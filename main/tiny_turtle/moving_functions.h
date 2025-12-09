#pragma once

#include <cstdint>

bool move(float distance, bool bounceAtObstacle);
void bounce();
void forward(float distance);
void back(float distance);
void turn(float degrees, int turningDirection);
void smartTurn(float angle);
void drawCoordinates(float scale);
bool spiral(double initialRadiusMM, double finalRadiusMM, double revolutions, bool clockwise, bool stopAtSwitch, bool stopAtSensor, bool togglePenAtSensor);
