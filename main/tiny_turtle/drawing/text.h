#pragma once
/**
 * @file drawing/text.h
 * @brief Text-Zeichenfunktionen
 */

#include <string>
#include <cstdint>

namespace tiny_turtle {
namespace drawing {

/**
 * @brief Text schreiben
 * @param text Der zu schreibende Text
 * @param scale Schriftgröße in mm
 */
void plotText(const std::string& text, int scale);

/**
 * @brief Einzelnes Zeichen schreiben
 * @param character ASCII-Zeichen
 * @param scale Schriftgröße
 */
void plotChar(uint8_t character, float scale);

/**
 * @brief ASCII zu Font-Index konvertieren
 * @param c ASCII-Zeichen
 * @return Index im Font-Array
 */
int asciiToFontIndex(uint8_t c);

}  // namespace drawing
}  // namespace tiny_turtle

// Legacy-Kompatibilität
void plotText(const std::string& str, int scale);
void plotChar(uint8_t character, float scale);
int ASCIItoIndex(uint8_t c);
