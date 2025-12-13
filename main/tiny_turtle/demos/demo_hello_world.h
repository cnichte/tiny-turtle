#pragma once
/**
 * @file demo_hello_world.h
 * @brief Demo: Schreibt "Hello World!" mit dem Plotter
 */

namespace tiny_turtle {
namespace demos {

/**
 * @brief Schreibt "Hello World!" in der angegebenen Zeichenhöhe
 * @param height Höhe der Buchstaben in mm (Standard: 15mm)
 */
void runHelloWorld(float height = 15.0f);

}  // namespace demos
}  // namespace tiny_turtle
