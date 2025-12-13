#pragma once

#include <algorithm>
#include <cstdint>

#include "driver/gpio.h"

// Minimal Arduino-like helpers implemented on ESP-IDF so the original sketch compiles.
constexpr int INPUT = 0;
constexpr int OUTPUT = 1;
constexpr int INPUT_PULLUP = 2;
constexpr int HIGH = 1;
constexpr int LOW = 0;

void pinMode(int pin, int mode);
void digitalWrite(int pin, int val);
int digitalRead(int pin);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
long random(long max);
long random(long min, long max);
inline long constrain(long x, long a, long b) { return std::clamp(x, a, b); }
int analogRead(int pin);
void tone(int pin, uint32_t freq_hz, uint32_t duration_ms);

class Servo {
 public:
  Servo();
  void attach(int pin);
  void write(int angle_deg);
  void detach();

 private:
  bool attached_;
  gpio_num_t pin_;
};
