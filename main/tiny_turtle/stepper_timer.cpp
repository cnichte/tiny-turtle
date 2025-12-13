#include "stepper_timer.h"
#include "low_level_hardware_functions.h"
#include "constants.h"
#include "esp_log.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "stepper_timer";

// Timer-Handle
static gptimer_handle_t stepper_timer = NULL;

// Motor-Zustand (volatile weil im Interrupt verwendet)
static volatile MotorCommand current_command = MotorCommand::STOP;
static volatile int motor1_direction = 0;
static volatile int motor2_direction = 0;
static volatile bool timer_running = false;
static volatile int32_t step_count = 0;

// Geschwindigkeits-Einstellungen
static volatile uint32_t current_speed_us = 2000;  // Default: 2ms pro Schritt
static volatile uint32_t target_speed_us = 2000;   // Zielgeschwindigkeit für Rampe
static volatile uint32_t start_speed_us = 5000;   // Startgeschwindigkeit (langsam)
static const uint32_t MIN_SPEED_US = 500;         // Max Geschwindigkeit (500µs = 2000 steps/s)
static const uint32_t MAX_SPEED_US = 10000;       // Min Geschwindigkeit (10ms = 100 steps/s)

// Rampen-Parameter
static volatile uint32_t ramp_steps = 0;          // Schritte für volle Rampe (0 = deaktiviert)
static volatile uint32_t ramp_step_counter = 0;   // Aktueller Schritt in der Rampe
static volatile bool ramping_up = false;          // Beschleunigung aktiv
static volatile bool ramping_down = false;        // Verzögerung aktiv
static volatile bool smooth_stop_requested = false;

//============================================================================
// Timer-Interrupt Handler (wird bei jedem Step aufgerufen)
//============================================================================
// Hilfsfunktion: Geschwindigkeitsintervall im ISR aktualisieren
static void IRAM_ATTR update_timer_alarm(uint32_t new_interval_us) {
    // Direkter Register-Zugriff wäre schneller, aber gptimer API ist sicherer
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = new_interval_us,
        .reload_count = 0,
        .flags = {
            .auto_reload_on_alarm = true,
        },
    };
    gptimer_set_alarm_action(stepper_timer, &alarm_config);
}

static bool IRAM_ATTR stepper_timer_isr(gptimer_handle_t timer,
                                         const gptimer_alarm_event_data_t *edata,
                                         void *user_ctx) {
    // Rampen-Verarbeitung
    if (ramping_up || ramping_down) {
        ramp_step_counter = ramp_step_counter + 1;  // Vermeidet deprecated volatile++ Warning
        
        if (ramp_step_counter >= ramp_steps) {
            // Rampe beendet
            current_speed_us = ramping_up ? target_speed_us : start_speed_us;
            ramping_up = false;
            ramping_down = false;
            ramp_step_counter = 0;
            
            if (smooth_stop_requested) {
                // Vollständig gestoppt
                smooth_stop_requested = false;
                current_command = MotorCommand::STOP;
                motor1_direction = 0;
                motor2_direction = 0;
            }
        } else {
            // Lineare Interpolation der Geschwindigkeit
            if (ramping_up) {
                // Beschleunigen: von start_speed zu target_speed
                uint32_t speed_diff = start_speed_us - target_speed_us;
                current_speed_us = start_speed_us - (speed_diff * ramp_step_counter / ramp_steps);
            } else {
                // Verzögern: von target_speed zu start_speed
                uint32_t speed_diff = start_speed_us - target_speed_us;
                current_speed_us = target_speed_us + (speed_diff * ramp_step_counter / ramp_steps);
            }
        }
        update_timer_alarm(current_speed_us);
    }
    
    if (current_command == MotorCommand::STOP) {
        return true;  // Nichts tun wenn gestoppt
    }
    
    // Schritte ausführen
    if (motor1_direction != 0) {
        switchStepper(1, motor1_direction);
    }
    if (motor2_direction != 0) {
        switchStepper(2, motor2_direction);
    }
    
    // Schrittzähler aktualisieren
    step_count += motor1_direction;
    
    return true;  // Alarm erneut aktivieren (auto-reload)
}

//============================================================================
// Motor-Richtungen basierend auf Befehl setzen
//============================================================================
static void update_motor_directions(MotorCommand cmd) {
    switch (cmd) {
        case MotorCommand::STOP:
            motor1_direction = 0;
            motor2_direction = 0;
            break;
        case MotorCommand::FORWARD:
            motor1_direction = 1;
            motor2_direction = 1;
            break;
        case MotorCommand::BACKWARD:
            motor1_direction = -1;
            motor2_direction = -1;
            break;
        case MotorCommand::SPIN_CW:
            motor1_direction = 1;
            motor2_direction = -1;
            break;
        case MotorCommand::SPIN_CCW:
            motor1_direction = -1;
            motor2_direction = 1;
            break;
    }
}

//============================================================================
// Timer initialisieren
//============================================================================
void stepper_timer_init(void) {
    ESP_LOGI(TAG, "Initialisiere GPTimer für Schrittmotoren...");
    
    // Timer-Konfiguration
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,  // 1 MHz = 1 µs Auflösung
        .intr_priority = 0,
        .flags = {
            .intr_shared = false,
            .allow_pd = false,
            .backup_before_sleep = false,
        },
    };
    
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &stepper_timer));
    
    // Alarm-Konfiguration (Interrupt alle X Mikrosekunden)
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = current_speed_us,
        .reload_count = 0,
        .flags = {
            .auto_reload_on_alarm = true,
        },
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(stepper_timer, &alarm_config));
    
    // Interrupt-Callback registrieren
    gptimer_event_callbacks_t cbs = {
        .on_alarm = stepper_timer_isr,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(stepper_timer, &cbs, NULL));
    
    // Timer aktivieren (aber noch nicht starten)
    ESP_ERROR_CHECK(gptimer_enable(stepper_timer));
    
    ESP_LOGI(TAG, "GPTimer initialisiert (Intervall: %lu µs)", current_speed_us);
}

//============================================================================
// Timer starten
//============================================================================
void stepper_timer_start(void) {
    if (!timer_running && stepper_timer != NULL) {
        ESP_ERROR_CHECK(gptimer_start(stepper_timer));
        timer_running = true;
        ESP_LOGI(TAG, "Timer gestartet");
    }
}

//============================================================================
// Timer stoppen
//============================================================================
void stepper_timer_stop(void) {
    if (timer_running && stepper_timer != NULL) {
        ESP_ERROR_CHECK(gptimer_stop(stepper_timer));
        timer_running = false;
        stopSteppers();  // Spulen stromlos schalten
        ESP_LOGI(TAG, "Timer gestoppt");
    }
}

//============================================================================
// Motor-Befehl setzen
//============================================================================
void stepper_set_command(MotorCommand cmd) {
    current_command = cmd;
    update_motor_directions(cmd);
    
    if (cmd == MotorCommand::STOP) {
        stopSteppers();
    }
    
    const char* cmd_names[] = {"STOP", "FORWARD", "BACKWARD", "SPIN_CW", "SPIN_CCW"};
    ESP_LOGI(TAG, "Befehl: %s (M1=%d, M2=%d)", 
             cmd_names[(int)cmd], motor1_direction, motor2_direction);
}

//============================================================================
// Geschwindigkeit setzen
//============================================================================
void stepper_set_speed(uint32_t step_interval_us) {
    // Grenzen prüfen
    if (step_interval_us < 500) step_interval_us = 500;    // Max ~2000 Steps/s
    if (step_interval_us > 10000) step_interval_us = 10000;  // Min ~100 Steps/s
    
    current_speed_us = step_interval_us;
    
    if (stepper_timer != NULL) {
        // Timer-Alarm aktualisieren
        gptimer_alarm_config_t alarm_config = {
            .alarm_count = current_speed_us,
            .reload_count = 0,
            .flags = {
                .auto_reload_on_alarm = true,
            },
        };
        
        // Timer kurz stoppen, Alarm setzen, wieder starten
        bool was_running = timer_running;
        if (was_running) {
            gptimer_stop(stepper_timer);
        }
        
        gptimer_set_alarm_action(stepper_timer, &alarm_config);
        
        if (was_running) {
            gptimer_start(stepper_timer);
        }
    }
    
    ESP_LOGI(TAG, "Geschwindigkeit: %lu µs/Schritt", current_speed_us);
}

//============================================================================
// Getter-Funktionen
//============================================================================
uint32_t stepper_get_speed(void) {
    return current_speed_us;
}

int32_t stepper_get_step_count(void) {
    return step_count;
}

void stepper_reset_step_count(void) {
    step_count = 0;
}

bool stepper_is_running(void) {
    return timer_running && (current_command != MotorCommand::STOP);
}

//============================================================================
// Rampen-Steuerung
//============================================================================

// Rampe aktivieren (Anzahl Schritte für volle Beschleunigung)
void stepper_set_ramp(uint32_t steps) {
    ramp_steps = steps;
    if (steps > 0) {
        ESP_LOGI(TAG, "Rampe aktiviert: %lu Schritte", steps);
    } else {
        ESP_LOGI(TAG, "Rampe deaktiviert");
    }
}

// Zielgeschwindigkeit mit Rampe setzen
void stepper_set_target_speed(uint32_t new_target_us) {
    // Grenzen prüfen
    if (new_target_us < MIN_SPEED_US) new_target_us = MIN_SPEED_US;
    if (new_target_us > MAX_SPEED_US) new_target_us = MAX_SPEED_US;
    
    if (ramp_steps == 0) {
        // Keine Rampe - direkt setzen
        stepper_set_speed(new_target_us);
        return;
    }
    
    target_speed_us = new_target_us;
    ramp_step_counter = 0;
    
    if (current_speed_us > target_speed_us) {
        // Beschleunigen (kleineres Intervall = schneller)
        start_speed_us = current_speed_us;
        ramping_up = true;
        ramping_down = false;
        ESP_LOGI(TAG, "Beschleunigung: %lu -> %lu µs", start_speed_us, target_speed_us);
    } else if (current_speed_us < target_speed_us) {
        // Verzögern (größeres Intervall = langsamer)
        start_speed_us = current_speed_us;
        ramping_down = true;
        ramping_up = false;
        ESP_LOGI(TAG, "Verzögerung: %lu -> %lu µs", start_speed_us, target_speed_us);
    }
}

// Sanft stoppen
void stepper_smooth_stop(void) {
    if (ramp_steps == 0 || current_command == MotorCommand::STOP) {
        // Keine Rampe oder bereits gestoppt
        stepper_set_command(MotorCommand::STOP);
        return;
    }
    
    // Verzögerungsrampe starten
    smooth_stop_requested = true;
    target_speed_us = current_speed_us;  // Aktuelle Geschwindigkeit merken
    start_speed_us = MAX_SPEED_US;       // Bis zum Stillstand verzögern
    ramp_step_counter = 0;
    ramping_down = true;
    ramping_up = false;
    
    ESP_LOGI(TAG, "Sanftes Stoppen gestartet (%lu Schritte)", ramp_steps);
}

// Prüfen ob Rampe aktiv
bool stepper_is_ramping(void) {
    return ramping_up || ramping_down;
}
