#pragma once
/**
 * @file demo_shapes.h
 * @brief Demo: Geometrische Formen zeichnen (Quadrat, Dreieck, Kreis, etc.)
 */

namespace tiny_turtle
{
    namespace demos
    {

        /**
         * @brief Zeichnet ein Quadrat
         * @param size Seitenlänge in mm
         */
        void drawSquare(float size = 100.0f);

        /**
         * @brief Zeichnet ein gleichseitiges Dreieck
         * @param size Seitenlänge in mm
         */
        void drawTriangle(float size = 100.0f);

        /**
         * @brief Zeichnet ein regelmäßiges Polygon
         * @param sides Anzahl der Seiten (3 = Dreieck, 4 = Quadrat, 6 = Hexagon, etc.)
         * @param size Seitenlänge in mm
         */
        void drawPolygon(int sides, float size = 50.0f);

        /**
         * @brief Zeichnet einen Kreis
         * @param radius Radius in mm
         */
        void drawCircle(float radius = 50.0f);

        /**
         * @brief Zeichnet eine Spirale
         * @param turns Anzahl der Umdrehungen
         * @param startRadius Startradius in mm
         * @param endRadius Endradius in mm
         */
        void drawSpiral(int turns = 3, float startRadius = 10.0f, float endRadius = 50.0f);

        /**
         * @brief Zeichnet einen Stern
         * @param points Anzahl der Zacken (5 = klassischer Stern)
         * @param outerRadius Äußerer Radius in mm
         * @param innerRadius Innerer Radius in mm
         */
        void drawStar(int points = 5, float outerRadius = 50.0f, float innerRadius = 20.0f);

    } // namespace demos
} // namespace tiny_turtle
