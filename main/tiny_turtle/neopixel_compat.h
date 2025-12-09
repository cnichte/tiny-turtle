#pragma once

#include <cstdint>
#include <vector>

#include "esp_err.h"

// Minimal NeoPixel helper for a small WS2812 strip.
class NeoPixel {
 public:
  NeoPixel(int pin, int pixels);
  esp_err_t begin();
  void clear();
  uint32_t Color(uint8_t r, uint8_t g, uint8_t b) const;
  void setPixelColor(int idx, uint32_t color);
  void show();

 private:
  int pin_;
  int count_;
  std::vector<uint8_t> buffer_;  // GRB
  void ensure_started();
  bool started_{false};
};
