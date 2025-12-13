#pragma once
/**
 * @file drawing/fonts.h
 * @brief Font-Daten für Text-Zeichnung
 *
 * Kodierung:
 * - Einer: Y-Koordinate
 * - Zehner: X-Koordinate
 * - Hunderter: Zeichnen (1) oder Bewegen (0)
 * - 200: Ende
 * - 222: Punkt zeichnen
 */

#include <cstdint>

namespace tiny_turtle
{
    namespace drawing
    {
        // Font-Koordinaten für alle Zeichen
        extern const uint8_t FONT_DATA[63][14];

    } // namespace drawing
} // namespace tiny_turtle

// Legacy-Kompatibilität
extern const uint8_t fontCoordinates[63][14];
