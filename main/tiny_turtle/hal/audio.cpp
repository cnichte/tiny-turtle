/**
 * @file hal/audio.cpp
 * @brief Implementierung der Audio-Funktionen
 */

#include "audio.h"
#include "led.h"
#include "../core/config.h"
#include "gpio_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace tiny_turtle
{
    namespace hal
    {
        void initAudio()
        {
            pinMode(config::SPEAKER_PIN, OUTPUT);
        }

        void playTone(uint32_t frequency, uint32_t durationMs)
        {
            tone(config::SPEAKER_PIN, frequency, durationMs);
        }

        void stopTone()
        {
            noTone(config::SPEAKER_PIN);
        }

        void playRandomMelody()
        {
            int freq = random(200, 1000);

            for (int i = 0; i < 3; i++)
            {
                freq *= random(2) == 0 ? 0.5 : 2;
                if (freq < 50)
                    freq *= 4;
                if (freq > 1500)
                    freq /= 4;

                setLedColor(random(5) * 20, random(5) * 20, random(5) * 20);
                playTone(freq, 100);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }

    } // namespace hal
} // namespace tiny_turtle

// Legacy-Kompatibilität
void triTone() { tiny_turtle::hal::playRandomMelody(); }
