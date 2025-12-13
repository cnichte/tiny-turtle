#pragma once

#include <cstdint>

// idf.py -p /dev/cu.usbmodem5AAF2844941 -b 115200 flash <<- geht
// idf.py -p /dev/cu.usbmodem59720627361 -b 115200 flash <<- geht nicht (download module error)
//--------------------------------------------------------------------------- IO pins
// NOTE: adjust these GPIO numbers to your wiring. A4/A5/A6/A7 are not defined in ESP-IDF,
// so we map them to plain GPIO numbers here.
constexpr int A4 = 4; // 14
constexpr int A5 = 5; // 15
constexpr int A6 = 6; // 16
constexpr int A7 = 2; // 17

constexpr int STEP_1A = 9; // 9
constexpr int STEP_1B = 10; // 10
constexpr int STEP_1C = 11; // 11 
constexpr int STEP_1D = 12; // 12

constexpr int STEP_2A = 8; // 8
constexpr int STEP_2B = 7; // 7
constexpr int STEP_2C = 6; // 6
constexpr int STEP_2D = 5; // D5 

constexpr int SERVO = 4; // D4
constexpr int SPEAKER = 3; // 3
constexpr int NEOPIXEL = 2; // 2
constexpr int SWITCH_F = A4;
constexpr int SWITCH_B = A5;
constexpr int SENSOR = A6;

//--------------------------------------------------------------------------- global robot geometry and speed constants etc.
constexpr float PI = 3.14159265359f;
constexpr uint16_t minDelay = 1000;               // minimum delay time between steps (in microseconds)
constexpr uint16_t maxDelay = 3000;               // maximum delay time between steps (in microseconds)
constexpr uint16_t rampValue = 5;                 // smaller value = longer speed ramp
constexpr int stepsPerRotation = 64 * 64;         // 64 steps per rotation and 1:64 gearbox
constexpr float wheelCircumference = 24.05f * PI;  // mm <- adjust if distances are off
constexpr float stepsPerMM = stepsPerRotation / wheelCircumference;
constexpr float wheelDistance = 33.5f;             // mm <- adjust if angles are off
constexpr float stepsPer360Rotation = wheelDistance * PI * stepsPerMM;
constexpr int servoPenDown = 0;     // servo angle pen down
constexpr int servoPenUp = 180;     // servo angle pen up
constexpr float sensorOffset = 16;  // distance between photo sensor pinhole and center of robot
