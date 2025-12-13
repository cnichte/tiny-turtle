/**
 * @file demo_shapes.cpp
 * @brief Demo: Geometrische Formen zeichnen
 */

#include "demo_shapes.h"
#include "../tiny_turtle.h"
#include "esp_log.h"

static const char *TAG = "demo_shapes";

namespace tiny_turtle
{
    namespace demos
    {

        void drawSquare(float size)
        {
            ESP_LOGI(TAG, "Zeichne Quadrat (%.1f mm)", size);

            hal::penDown();
            for (int i = 0; i < 4; i++)
            {
                motion::move(size);
                motion::turn(90);
            }
            hal::penUp();

            ESP_LOGI(TAG, "Quadrat fertig.");
        }

        void drawTriangle(float size)
        {
            ESP_LOGI(TAG, "Zeichne Dreieck (%.1f mm)", size);

            hal::penDown();
            for (int i = 0; i < 3; i++)
            {
                motion::move(size);
                motion::turn(120); // 180 - 60 = 120° Außenwinkel
            }
            hal::penUp();

            ESP_LOGI(TAG, "Dreieck fertig.");
        }

        void drawPolygon(int sides, float size)
        {
            if (sides < 3)
                sides = 3;

            ESP_LOGI(TAG, "Zeichne %d-Eck (%.1f mm)", sides, size);

            float angle = 360.0f / sides;

            hal::penDown();
            for (int i = 0; i < sides; i++)
            {
                motion::move(size);
                motion::turn(angle);
            }
            hal::penUp();

            ESP_LOGI(TAG, "Polygon fertig.");
        }

        void drawCircle(float radius)
        {
            ESP_LOGI(TAG, "Zeichne Kreis (Radius %.1f mm)", radius);

            // Kreis als 36-Eck approximieren
            constexpr int segments = 36;
            float angle = 360.0f / segments;

            // Umfang / Segmente = Schrittlänge
            float circumference = 2.0f * config::PI * radius;
            float stepLength = circumference / segments;

            hal::penDown();
            for (int i = 0; i < segments; i++)
            {
                motion::move(stepLength);
                motion::turn(angle);
            }
            hal::penUp();

            ESP_LOGI(TAG, "Kreis fertig.");
        }

        void drawSpiral(int turns, float startRadius, float endRadius)
        {
            ESP_LOGI(TAG, "Zeichne Spirale (%d Umdrehungen)", turns);

            motion::spiral(turns, startRadius, endRadius);

            ESP_LOGI(TAG, "Spirale fertig.");
        }

        void drawStar(int points, float outerRadius, float innerRadius)
        {
            if (points < 3)
                points = 5;

            ESP_LOGI(TAG, "Zeichne %d-zackigen Stern", points);

            // Winkel zwischen den Zacken
            float angleStep = 360.0f / points;

            // Zum Startpunkt fahren (oben auf dem äußeren Radius)
            motion::move(outerRadius);
            motion::turn(90);

            hal::penDown();

            for (int i = 0; i < points; i++)
            {
                // Zum inneren Punkt
                motion::turn(-angleStep / 2.0f - 90.0f);
                float innerDist = outerRadius - innerRadius;
                motion::move(innerDist);

                // Zum nächsten äußeren Punkt
                motion::turn(angleStep);
                motion::move(innerDist);
                motion::turn(90.0f - angleStep / 2.0f);
            }

            hal::penUp();

            ESP_LOGI(TAG, "Stern fertig.");
        }

    } // namespace demos
} // namespace tiny_turtle
