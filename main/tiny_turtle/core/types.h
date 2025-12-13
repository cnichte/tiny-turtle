#pragma once
/**
 * @file types.h
 * @brief Gemeinsame Typdefinitionen für Tiny Turtle
 */

#include <cstdint>

namespace tiny_turtle
{

    //===========================================================================
    // Bewegungs-Richtungen
    //===========================================================================

    enum class Direction : int8_t
    {
        BACKWARD = -1,
        STOP = 0,
        FORWARD = 1
    };

    enum class TurnDirection : int8_t
    {
        COUNTER_CLOCKWISE = -1,
        CLOCKWISE = 1
    };

    //===========================================================================
    // Motor-Befehle (für Timer-basierte Steuerung)
    //===========================================================================

    enum class MotorCommand
    {
        STOP,     // Motoren stoppen
        FORWARD,  // Beide Motoren vorwärts
        BACKWARD, // Beide Motoren rückwärts
        SPIN_CW,  // Drehung auf der Stelle (Uhrzeigersinn)
        SPIN_CCW, // Drehung auf der Stelle (gegen Uhrzeigersinn)
    };

    //===========================================================================
    // Stift-Zustand
    //===========================================================================

    enum class PenState : uint8_t
    {
        UP = 0,
        DOWN = 1
    };

    //===========================================================================
    // Sensor-Zustände
    //===========================================================================

    struct BumperState
    {
        bool front;
        bool back;

        bool any() const { return front || back; }
    };

    //===========================================================================
    // 2D-Koordinaten
    //===========================================================================

    struct Point2D
    {
        float x;
        float y;

        Point2D() : x(0), y(0) {}
        Point2D(float x_, float y_) : x(x_), y(y_) {}
    };

    struct Coordinate
    {
        uint8_t x;
        uint8_t y;
        bool draw;
    };

} // namespace tiny_turtle

// Legacy-Kompatibilität
using MotorCommand = tiny_turtle::MotorCommand;
