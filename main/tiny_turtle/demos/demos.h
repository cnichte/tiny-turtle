#pragma once
/**
 * @file demos.h
 * @brief Sammlung aller Demo-Programme für Tiny Turtle
 * 
 * Inkludieren Sie diese Datei für Zugriff auf alle Demos.
 * 
 * Verwendung in tiny_turtle_app.cpp:
 * @code
 * #include "tiny_turtle/demos/demos.h"
 * 
 * void app_main() {
 *     turtleInit();
 *     
 *     // Eine Demo auswählen:
 *     tiny_turtle::demos::runHelloWorld();
 *     // tiny_turtle::demos::runMotorTest();
 *     // tiny_turtle::demos::drawSquare(100);
 *     // tiny_turtle::demos::drawCircle(50);
 * }
 * @endcode
 */

#include "demo_hello_world.h"
#include "demo_motor_test.h"
#include "demo_shapes.h"

namespace tiny_turtle {
namespace demos {

/**
 * @brief Liste aller verfügbaren Demos
 */
enum class DemoType {
    HELLO_WORLD,      ///< Schreibt "Hello World!"
    MOTOR_TEST,       ///< Motor-Test mit Rampen
    SIMPLE_ROTATION,  ///< Einfacher Dauerrotationstest
    SQUARE,           ///< Zeichnet ein Quadrat
    TRIANGLE,         ///< Zeichnet ein Dreieck
    CIRCLE,           ///< Zeichnet einen Kreis
    SPIRAL,           ///< Zeichnet eine Spirale
    STAR,             ///< Zeichnet einen Stern
};

/**
 * @brief Führt eine Demo anhand ihres Typs aus
 * @param demo Der Demo-Typ
 */
inline void runDemo(DemoType demo) {
    switch (demo) {
        case DemoType::HELLO_WORLD:
            runHelloWorld();
            break;
        case DemoType::MOTOR_TEST:
            runMotorTest();
            break;
        case DemoType::SIMPLE_ROTATION:
            runSimpleRotationTest();
            break;
        case DemoType::SQUARE:
            drawSquare();
            break;
        case DemoType::TRIANGLE:
            drawTriangle();
            break;
        case DemoType::CIRCLE:
            drawCircle();
            break;
        case DemoType::SPIRAL:
            drawSpiral();
            break;
        case DemoType::STAR:
            drawStar();
            break;
    }
}

}  // namespace demos
}  // namespace tiny_turtle
