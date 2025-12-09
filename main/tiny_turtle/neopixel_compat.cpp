#include "neopixel_compat.h"

#include <algorithm>

#include "esp_log.h"

namespace {
constexpr const char* TAG = "neopixel";
}

NeoPixel::NeoPixel(int pin, int pixels) : pin_(pin), count_(pixels), buffer_(pixels * 3, 0) {}

esp_err_t NeoPixel::begin() {
  started_ = true;
  ESP_LOGW(TAG, "NeoPixel output is stubbed (no hardware driver); GPIO %d ignored", pin_);
  return ESP_OK;
}

void NeoPixel::ensure_started() {
  if (!started_) {
    begin();
  }
}

void NeoPixel::clear() {
  ensure_started();
  std::fill(buffer_.begin(), buffer_.end(), 0);
}

uint32_t NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b) const {
  return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | b;
}

void NeoPixel::setPixelColor(int idx, uint32_t color) {
  ensure_started();
  if (idx < 0 || idx >= count_) return;
  buffer_[idx * 3 + 0] = (color >> 16) & 0xFF;  // R
  buffer_[idx * 3 + 1] = (color >> 8) & 0xFF;   // G
  buffer_[idx * 3 + 2] = color & 0xFF;          // B
}

void NeoPixel::show() {
  ensure_started();
  // No-op in stub implementation.
}
