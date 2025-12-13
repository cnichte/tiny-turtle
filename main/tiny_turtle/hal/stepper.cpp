/**
 * @file hal/stepper.cpp
 * @brief Implementierung der Schrittmotor-Steuerung
 */

#include "stepper.h"
#include "../core/config.h"
#include "../core/globals.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "hal.stepper";

namespace tiny_turtle {
namespace hal {

//===========================================================================
// Statische Daten (IRAM für ISR-Zugriff)
//===========================================================================

static const DRAM_ATTR uint8_t STEPPER_PINS[2][4] = {
    { config::STEP_1A, config::STEP_1B, config::STEP_1C, config::STEP_1D },
    { config::STEP_2A, config::STEP_2B, config::STEP_2C, config::STEP_2D }
};

static const DRAM_ATTR uint8_t HALF_STEP_PATTERN[8][4] = {
    { 1, 0, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 }
};

// Timer-Handle
static gptimer_handle_t s_timer = nullptr;

// Motor-Zustand (volatile für ISR)
static volatile MotorCommand s_command = MotorCommand::STOP;
static volatile int s_motor1_dir = 0;
static volatile int s_motor2_dir = 0;
static volatile bool s_timer_running = false;
static volatile int32_t s_step_count = 0;

// Geschwindigkeits-Einstellungen
static volatile uint32_t s_current_speed_us = config::DEFAULT_STEP_DELAY_US;
static volatile uint32_t s_target_speed_us = config::DEFAULT_STEP_DELAY_US;
static volatile uint32_t s_start_speed_us = 5000;
static constexpr uint32_t MIN_SPEED_US = 500;
static constexpr uint32_t MAX_SPEED_US = 10000;

// Rampen-Parameter
static volatile uint32_t s_ramp_steps = 0;
static volatile uint32_t s_ramp_counter = 0;
static volatile bool s_ramping_up = false;
static volatile bool s_ramping_down = false;
static volatile bool s_smooth_stop = false;

//===========================================================================
// Low-Level Motor-Funktionen
//===========================================================================

void IRAM_ATTR stepMotor(uint8_t stepper, int direction) {
    uint8_t idx = stepper - 1;
    
    if (stepper == 1) {
        stepCount1 = (8 + direction + stepCount1) % 8;
        const uint8_t* pattern = HALF_STEP_PATTERN[stepCount1];
        for (int i = 0; i < 4; i++) {
            gpio_set_level(static_cast<gpio_num_t>(STEPPER_PINS[idx][i]), pattern[i]);
        }
    } else {
        stepCount2 = (8 + direction + stepCount2) % 8;
        const uint8_t* pattern = HALF_STEP_PATTERN[stepCount2];
        for (int i = 0; i < 4; i++) {
            gpio_set_level(static_cast<gpio_num_t>(STEPPER_PINS[idx][i]), pattern[i]);
        }
    }
}

void IRAM_ATTR stopMotors() {
    for (int i = 0; i < 4; i++) {
        gpio_set_level(static_cast<gpio_num_t>(STEPPER_PINS[0][i]), 0);
        gpio_set_level(static_cast<gpio_num_t>(STEPPER_PINS[1][i]), 0);
    }
}

//===========================================================================
// Timer ISR
//===========================================================================

static void IRAM_ATTR updateTimerAlarm(uint32_t intervalUs) {
    gptimer_alarm_config_t cfg = {
        .alarm_count = intervalUs,
        .reload_count = 0,
        .flags = { .auto_reload_on_alarm = true }
    };
    gptimer_set_alarm_action(s_timer, &cfg);
}

static bool IRAM_ATTR timerISR(gptimer_handle_t timer,
                                const gptimer_alarm_event_data_t* edata,
                                void* ctx) {
    // Rampen-Verarbeitung
    if (s_ramping_up || s_ramping_down) {
        s_ramp_counter = s_ramp_counter + 1;
        
        if (s_ramp_counter >= s_ramp_steps) {
            s_current_speed_us = s_ramping_up ? s_target_speed_us : s_start_speed_us;
            s_ramping_up = false;
            s_ramping_down = false;
            s_ramp_counter = 0;
            
            if (s_smooth_stop) {
                s_smooth_stop = false;
                s_command = MotorCommand::STOP;
                s_motor1_dir = 0;
                s_motor2_dir = 0;
            }
        } else {
            uint32_t diff = s_start_speed_us - s_target_speed_us;
            if (s_ramping_up) {
                s_current_speed_us = s_start_speed_us - (diff * s_ramp_counter / s_ramp_steps);
            } else {
                s_current_speed_us = s_target_speed_us + (diff * s_ramp_counter / s_ramp_steps);
            }
        }
        updateTimerAlarm(s_current_speed_us);
    }
    
    if (s_command == MotorCommand::STOP) {
        return true;
    }
    
    if (s_motor1_dir != 0) stepMotor(1, s_motor1_dir);
    if (s_motor2_dir != 0) stepMotor(2, s_motor2_dir);
    
    s_step_count += s_motor1_dir;
    
    return true;
}

//===========================================================================
// Motor-Richtungen
//===========================================================================

static void updateMotorDirections(MotorCommand cmd) {
    switch (cmd) {
        case MotorCommand::STOP:
            s_motor1_dir = 0;
            s_motor2_dir = 0;
            break;
        case MotorCommand::FORWARD:
            s_motor1_dir = 1;
            s_motor2_dir = 1;
            break;
        case MotorCommand::BACKWARD:
            s_motor1_dir = -1;
            s_motor2_dir = -1;
            break;
        case MotorCommand::SPIN_CW:
            s_motor1_dir = 1;
            s_motor2_dir = -1;
            break;
        case MotorCommand::SPIN_CCW:
            s_motor1_dir = -1;
            s_motor2_dir = 1;
            break;
    }
}

//===========================================================================
// Timer-Steuerung
//===========================================================================

void initStepperTimer() {
    ESP_LOGI(TAG, "Initialisiere GPTimer...");
    
    gptimer_config_t cfg = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,
        .intr_priority = 0,
        .flags = {
            .intr_shared = false,
            .allow_pd = false,
            .backup_before_sleep = false,
        }
    };
    
    ESP_ERROR_CHECK(gptimer_new_timer(&cfg, &s_timer));
    
    gptimer_alarm_config_t alarm = {
        .alarm_count = s_current_speed_us,
        .reload_count = 0,
        .flags = { .auto_reload_on_alarm = true }
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(s_timer, &alarm));
    
    gptimer_event_callbacks_t cbs = { .on_alarm = timerISR };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(s_timer, &cbs, nullptr));
    ESP_ERROR_CHECK(gptimer_enable(s_timer));
    
    ESP_LOGI(TAG, "GPTimer initialisiert (%lu µs)", s_current_speed_us);
}

void startStepperTimer() {
    if (!s_timer_running && s_timer) {
        ESP_ERROR_CHECK(gptimer_start(s_timer));
        s_timer_running = true;
        ESP_LOGI(TAG, "Timer gestartet");
    }
}

void stopStepperTimer() {
    if (s_timer_running && s_timer) {
        ESP_ERROR_CHECK(gptimer_stop(s_timer));
        s_timer_running = false;
        stopMotors();
        ESP_LOGI(TAG, "Timer gestoppt");
    }
}

void setMotorCommand(MotorCommand cmd) {
    s_command = cmd;
    updateMotorDirections(cmd);
    
    if (cmd == MotorCommand::STOP) {
        stopMotors();
    }
    
    static const char* names[] = {"STOP", "FORWARD", "BACKWARD", "SPIN_CW", "SPIN_CCW"};
    ESP_LOGI(TAG, "Befehl: %s (M1=%d, M2=%d)", names[static_cast<int>(cmd)], s_motor1_dir, s_motor2_dir);
}

void setStepSpeed(uint32_t intervalUs) {
    if (intervalUs < MIN_SPEED_US) intervalUs = MIN_SPEED_US;
    if (intervalUs > MAX_SPEED_US) intervalUs = MAX_SPEED_US;
    
    s_current_speed_us = intervalUs;
    
    if (s_timer) {
        bool wasRunning = s_timer_running;
        if (wasRunning) gptimer_stop(s_timer);
        
        gptimer_alarm_config_t alarm = {
            .alarm_count = s_current_speed_us,
            .reload_count = 0,
            .flags = { .auto_reload_on_alarm = true }
        };
        gptimer_set_alarm_action(s_timer, &alarm);
        
        if (wasRunning) gptimer_start(s_timer);
    }
    
    ESP_LOGI(TAG, "Geschwindigkeit: %lu µs/Schritt", s_current_speed_us);
}

uint32_t getStepSpeed() { return s_current_speed_us; }
int32_t getStepCount() { return s_step_count; }
void resetStepCount() { s_step_count = 0; }
bool isMotorRunning() { return s_timer_running && (s_command != MotorCommand::STOP); }

//===========================================================================
// Rampen-Steuerung
//===========================================================================

void setRamp(uint32_t steps) {
    s_ramp_steps = steps;
    ESP_LOGI(TAG, "Rampe: %lu Schritte", steps);
}

void setTargetSpeed(uint32_t targetUs) {
    if (targetUs < MIN_SPEED_US) targetUs = MIN_SPEED_US;
    if (targetUs > MAX_SPEED_US) targetUs = MAX_SPEED_US;
    
    if (s_ramp_steps == 0) {
        setStepSpeed(targetUs);
        return;
    }
    
    s_target_speed_us = targetUs;
    s_ramp_counter = 0;
    
    if (s_current_speed_us > targetUs) {
        s_start_speed_us = s_current_speed_us;
        s_ramping_up = true;
        s_ramping_down = false;
        ESP_LOGI(TAG, "Beschleunigung: %lu -> %lu µs", s_start_speed_us, s_target_speed_us);
    } else if (s_current_speed_us < targetUs) {
        s_start_speed_us = s_current_speed_us;
        s_ramping_down = true;
        s_ramping_up = false;
        ESP_LOGI(TAG, "Verzögerung: %lu -> %lu µs", s_start_speed_us, s_target_speed_us);
    }
}

bool isRamping() { return s_ramping_up || s_ramping_down; }

void smoothStop() {
    if (s_ramp_steps == 0 || s_command == MotorCommand::STOP) {
        setMotorCommand(MotorCommand::STOP);
        return;
    }
    
    s_smooth_stop = true;
    s_target_speed_us = s_current_speed_us;
    s_start_speed_us = MAX_SPEED_US;
    s_ramp_counter = 0;
    s_ramping_down = true;
    s_ramping_up = false;
    
    ESP_LOGI(TAG, "Sanftes Stoppen (%lu Schritte)", s_ramp_steps);
}

}  // namespace hal
}  // namespace tiny_turtle

//===========================================================================
// Legacy-Kompatibilität
//===========================================================================

void IRAM_ATTR switchStepper(uint8_t stepper, int direction) {
    tiny_turtle::hal::stepMotor(stepper, direction);
}

void IRAM_ATTR stopSteppers() {
    tiny_turtle::hal::stopMotors();
}

void stepper_timer_init() { tiny_turtle::hal::initStepperTimer(); }
void stepper_timer_start() { tiny_turtle::hal::startStepperTimer(); }
void stepper_timer_stop() { tiny_turtle::hal::stopStepperTimer(); }
void stepper_set_command(MotorCommand cmd) { tiny_turtle::hal::setMotorCommand(cmd); }
void stepper_set_speed(uint32_t us) { tiny_turtle::hal::setStepSpeed(us); }
uint32_t stepper_get_speed() { return tiny_turtle::hal::getStepSpeed(); }
int32_t stepper_get_step_count() { return tiny_turtle::hal::getStepCount(); }
void stepper_reset_step_count() { tiny_turtle::hal::resetStepCount(); }
bool stepper_is_running() { return tiny_turtle::hal::isMotorRunning(); }
void stepper_set_ramp(uint32_t steps) { tiny_turtle::hal::setRamp(steps); }
void stepper_set_target_speed(uint32_t us) { tiny_turtle::hal::setTargetSpeed(us); }
bool stepper_is_ramping() { return tiny_turtle::hal::isRamping(); }
void stepper_smooth_stop() { tiny_turtle::hal::smoothStop(); }
