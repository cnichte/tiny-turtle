/**
 * @file drawing/text.cpp
 * @brief Text-Zeichenfunktionen Implementierung
 */

#include "text.h"
#include "fonts.h"
#include "../motion/motion.h"
#include "../hal/servo.h"
#include "../core/config.h"
#include <cmath>

namespace tiny_turtle
{
    namespace drawing
    {

        int asciiToFontIndex(uint8_t c)
        {
            if (c >= 'A' && c <= 'Z')
                return c - 'A';
            if (c >= 'a' && c <= 'z')
                return c - 'a';
            if (c >= '0' && c <= '9')
                return c - '0' + 26;
            if (c == ' ')
                return 36; // Space
            if (c == '*')
                return 38; // Strich
            if (c == 196)
                return 39; // Ä
            if (c == 214)
                return 40; // Ö
            if (c == 220)
                return 41; // Ü
            if (c == ',')
                return 42;
            if (c == '-')
                return 43;
            if (c == '.')
                return 44;
            if (c == '!')
                return 45;
            if (c == '?')
                return 46;
            if (c == 223)
                return 47; // ß
            if (c == '\'')
                return 48;
            if (c == '&')
                return 49;
            if (c == '+')
                return 50;
            if (c == ':')
                return 51;
            if (c == ';')
                return 52;
            if (c == '"')
                return 53;
            if (c == '#')
                return 54;
            if (c == '(')
                return 55;
            if (c == ')')
                return 56;
            if (c == '=')
                return 57;
            if (c == '@')
                return 58;
            if (c == 228)
                return 39; // ä -> Ä
            if (c == 246)
                return 40; // ö -> Ö
            if (c == 252)
                return 41; // ü -> Ü
            return 37;     // Unknown -> Space
        }

        void plotChar(uint8_t character, float scale)
        {
            int index = asciiToFontIndex(character);

            for (int i = 0; i < 14; i++)
            {
                uint8_t coord = FONT_DATA[index][i];

                if (coord == 200)
                {
                    // Ende des Zeichens
                    break;
                }

                if (coord == 222)
                {
                    // Punkt zeichnen
                    hal::penDown();
                    hal::penUp();
                    continue;
                }

                uint8_t x = (coord / 10) % 10; // Zehner = X
                uint8_t y = coord % 10;        // Einer = Y
                bool draw = (coord >= 100);    // Hunderter = Zeichnen

                // Distanz berechnen
                float dist = sqrt((float)(x * x + y * y)) * scale;
                float angle = atan2((float)x, (float)y) * 180.0f / M_PI;

                if (draw)
                {
                    hal::penDown();
                }
                else
                {
                    hal::penUp();
                }

                // Bewegung zum Punkt
                if (dist > 0.1f)
                {
                    motion::turn(angle);
                    motion::forward(dist);
                    motion::turn(-angle);
                }
            }

            // Zeichenabstand
            hal::penUp();
            motion::forward(5.0f * scale);
        }

        void plotText(const std::string &text, int scale)
        {
            for (size_t i = 0; i < text.length(); i++)
            {
                plotChar(static_cast<uint8_t>(text[i]), static_cast<float>(scale));
            }
        }

    } // namespace drawing
} // namespace tiny_turtle

// Legacy-Kompatibilität
void plotText(const std::string &str, int scale)
{
    tiny_turtle::drawing::plotText(str, scale);
}

void plotChar(uint8_t character, float scale)
{
    tiny_turtle::drawing::plotChar(character, scale);
}

int ASCIItoIndex(uint8_t c)
{
    return tiny_turtle::drawing::asciiToFontIndex(c);
}
