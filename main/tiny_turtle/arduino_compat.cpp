#include "arduino_compat.h"

#include <cmath>

#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h" // https://wolles-elektronikkiste.de/freertos-mit-esp32-und-arduino-nutzen
#include "freertos/task.h"

namespace {
constexpr const char* TAG = "arduino_compat";

// Servo uses LEDC timer/channel 0 at 50Hz.
constexpr ledc_timer_t kServoTimer = LEDC_TIMER_0;
constexpr ledc_channel_t kServoChannel = LEDC_CHANNEL_0;
constexpr ledc_timer_bit_t kServoResolution = LEDC_TIMER_15_BIT;  // 20 ms period, enough granularity
constexpr uint32_t kServoFreq = 50;

// Speaker (tone) uses LEDC timer/channel 1.
constexpr ledc_timer_t kToneTimer = LEDC_TIMER_1;
constexpr ledc_channel_t kToneChannel = LEDC_CHANNEL_1;
constexpr ledc_timer_bit_t kToneResolution = LEDC_TIMER_10_BIT;

adc_oneshot_unit_handle_t adc_handle = nullptr;
bool adc_initialized = false;

void init_adc() {
  if (adc_initialized) return;
  adc_oneshot_unit_init_cfg_t init_cfg = {
      .unit_id = ADC_UNIT_1,
      .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  if (adc_oneshot_new_unit(&init_cfg, &adc_handle) == ESP_OK) {
    adc_initialized = true;
  } else {
    ESP_LOGE(TAG, "Failed to init ADC");
  }
}

void configure_ledc_timer(ledc_timer_t timer, ledc_timer_bit_t resolution, uint32_t freq_hz) {
  ledc_timer_config_t config = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = resolution,
      .timer_num = timer,
      .freq_hz = freq_hz,
      .clk_cfg = LEDC_AUTO_CLK,
  };
  ledc_timer_config(&config);
}

uint32_t duty_from_pulse_us(uint32_t pulse_us) {
  // Convert microseconds pulse (e.g. 500-2500us) to LEDC duty for the servo timer.
  const uint32_t period_us = 1000000 / kServoFreq;  // 20,000us
  const uint32_t max_duty = (1u << kServoResolution) - 1;
  return (pulse_us * max_duty) / period_us;
}
}  // namespace

// GPIO helpers
void pinMode(int pin, int mode) {
  gpio_config_t cfg = {};
  cfg.pin_bit_mask = 1ULL << pin;
  cfg.mode = (mode == OUTPUT) ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;
  if (mode == INPUT_PULLUP) {
    cfg.pull_up_en = GPIO_PULLUP_ENABLE;
  }
  gpio_config(&cfg);
}

void digitalWrite(int pin, int val) { gpio_set_level(static_cast<gpio_num_t>(pin), val ? 1 : 0); }

int digitalRead(int pin) { return gpio_get_level(static_cast<gpio_num_t>(pin)); }

void delay(uint32_t ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }

void delayMicroseconds(uint32_t us) { esp_rom_delay_us(us); }

long random(long max) { return static_cast<long>(esp_random() % static_cast<uint32_t>(max)); }

long random(long min, long max) {
  if (max <= min) return min;
  return min + static_cast<long>(esp_random() % static_cast<uint32_t>(max - min));
}

// ADC helper (uses ADC1 by default)
int analogRead(int pin) {
  init_adc();
  if (!adc_initialized || !adc_handle) return 0;

  adc_unit_t unit;
  adc_channel_t channel;
  if (adc_oneshot_io_to_channel(static_cast<gpio_num_t>(pin), &unit, &channel) != ESP_OK) {
    ESP_LOGW(TAG, "GPIO %d not ADC-capable", pin);
    return 0;
  }
  if (unit != ADC_UNIT_1) {
    ESP_LOGW(TAG, "Only ADC1 supported in this helper (gpio %d)", pin);
    return 0;
  }

  adc_oneshot_chan_cfg_t cfg = {
      .atten = ADC_ATTEN_DB_11,
      .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  adc_oneshot_config_channel(adc_handle, channel, &cfg);

  int raw = 0;
  if (adc_oneshot_read(adc_handle, channel, &raw) != ESP_OK) {
    ESP_LOGW(TAG, "ADC read failed on gpio %d", pin);
    return 0;
  }
  return raw;
}

// Servo implementation
Servo::Servo() : attached_(false), pin_(GPIO_NUM_NC) {}

void Servo::attach(int pin) {
  if (!attached_) {
    configure_ledc_timer(kServoTimer, kServoResolution, kServoFreq);
    ledc_channel_config_t ch_cfg = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = kServoChannel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = kServoTimer,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&ch_cfg);
  }
  attached_ = true;
  pin_ = static_cast<gpio_num_t>(pin);
}

void Servo::write(int angle_deg) {
  if (!attached_) return;
  int clamped_angle = std::clamp(angle_deg, 0, 180);
  // Typical servo pulse: 500us (0°) to 2500us (180°)
  uint32_t pulse = 500 + ((2000 * clamped_angle) / 180);
  uint32_t duty = duty_from_pulse_us(pulse);
  ledc_set_duty(LEDC_LOW_SPEED_MODE, kServoChannel, duty);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, kServoChannel);
}

void Servo::detach() {
  if (!attached_) return;
  ledc_stop(LEDC_LOW_SPEED_MODE, kServoChannel, 0);
  attached_ = false;
}

// tone implementation using LEDC
void tone(int pin, uint32_t freq_hz, uint32_t duration_ms) {
  configure_ledc_timer(kToneTimer, kToneResolution, freq_hz);
  ledc_channel_config_t ch_cfg = {
      .gpio_num = pin,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = kToneChannel,
      .intr_type = LEDC_INTR_DISABLE,
      .timer_sel = kToneTimer,
      .duty = (1u << kToneResolution) / 2,  // 50% duty square wave
      .hpoint = 0,
  };
  ledc_channel_config(&ch_cfg);

  vTaskDelay(pdMS_TO_TICKS(duration_ms));
  ledc_stop(LEDC_LOW_SPEED_MODE, kToneChannel, 0);
}
