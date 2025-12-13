#pragma once
/**
 * @file hal/audio.h
 * @brief Hardware Abstraction Layer für Audio (Speaker/Buzzer)
 */

#include <cstdint>

namespace tiny_turtle {
namespace hal {

/**
 * @brief Audio initialisieren
 */
void initAudio();

/**
 * @brief Ton abspielen
 * @param frequency Frequenz in Hz
 * @param durationMs Dauer in Millisekunden
 */
void playTone(uint32_t frequency, uint32_t durationMs);

/**
 * @brief Ton stoppen
 */
void stopTone();

/**
 * @brief Zufällige Melodie abspielen (3 Töne)
 */
void playRandomMelody();

}  // namespace hal
}  // namespace tiny_turtle

// Legacy-Kompatibilität
void triTone();
void tone(int pin, uint32_t frequency, uint32_t duration);
void noTone(int pin);
