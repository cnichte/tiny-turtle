#pragma once
/**
 * @file neopixel.h
 * @brief WS2812B NeoPixel LED-Treiber für ESP32
 *
 * Verwendet das RMT-Peripheral für präzises Timing.
 * NeoPixel ist der Markenname von Adafruit für WS2812B RGB-LED-Chips.
 */

#include <cstdint>
#include <vector>

#include "driver/rmt_tx.h"
#include "esp_err.h"

/**
 * @brief NeoPixel (WS2812B) LED-Controller
 *
 * Steuert adressierbare RGB-LEDs über das RMT-Peripheral des ESP32.
 */
class NeoPixel
{
public:
  /**
   * @brief Konstruktor
   * @param pin GPIO-Pin für die Datenleitung
   * @param pixels Anzahl der LEDs in der Kette
   */
  NeoPixel(int pin, int pixels);

  /**
   * @brief Destruktor - gibt RMT-Ressourcen frei
   */
  ~NeoPixel();

  /**
   * @brief Initialisiert das RMT-Peripheral
   * @return ESP_OK bei Erfolg
   */
  esp_err_t begin();

  /**
   * @brief Setzt alle LEDs auf Schwarz (aus)
   */
  void clear();

  /**
   * @brief Erstellt einen 24-Bit Farbwert aus RGB-Komponenten
   * @param r Rot (0-255)
   * @param g Grün (0-255)
   * @param b Blau (0-255)
   * @return 24-Bit Farbwert (0xRRGGBB)
   */
  uint32_t Color(uint8_t r, uint8_t g, uint8_t b) const;

  /**
   * @brief Setzt die Farbe einer einzelnen LED
   * @param idx LED-Index (0-basiert)
   * @param color 24-Bit Farbwert
   */
  void setPixelColor(int idx, uint32_t color);

  /**
   * @brief Überträgt die Farben an die LEDs
   */
  void show();

  /**
   * @brief Gibt die Anzahl der LEDs zurück
   */
  int numPixels() const { return count_; }

private:
  int pin_;
  int count_;
  std::vector<uint8_t> buffer_; // RGB-Daten

  // RMT-Handles
  rmt_channel_handle_t rmt_channel_{nullptr};
  rmt_encoder_handle_t rmt_encoder_{nullptr};
  bool started_{false};

  void ensure_started();
};
