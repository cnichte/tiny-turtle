#include <cmath>
#include <string>

#include "arduino_compat.h"
#include "constants.h"
#include "coordinates.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "neopixel_compat.h"
#include "tiny_turtle.h"
#include "stepper_timer.h"  // NEU: Timer-basierte Motor-Steuerung


static const char* TAG = "tiny_turtle";

NeoPixel pixels(NEOPIXEL, 1);

Servo myservo;           //                                                   servo to lift / lower the pen
uint8_t stepCount1 = 0;  //                                                   global sep counter motor 1
uint8_t stepCount2 = 0;  //                                                   global sep counter motor 2

long delayValue = 2000;  //                                                   current delay between steps
int direction = 1;       //                                                   current moving direction
bool drawing = true;     //                                                   pen position (true = pen is down / false = pen is up)

//=========================================================================== SETUP
void setup() {
  pixels.begin();
  pixels.clear();

  ESP_LOGI(TAG, "Hi there!");

  pinMode(STEP_1A, OUTPUT);
  pinMode(STEP_1B, OUTPUT);
  pinMode(STEP_1C, OUTPUT);
  pinMode(STEP_1D, OUTPUT);

  pinMode(STEP_2A, OUTPUT);
  pinMode(STEP_2B, OUTPUT);
  pinMode(STEP_2C, OUTPUT);
  pinMode(STEP_2D, OUTPUT);

  pinMode(SERVO, OUTPUT);
  pinMode(SPEAKER, OUTPUT);

  pinMode(SWITCH_F, INPUT_PULLUP);
  pinMode(SWITCH_B, INPUT_PULLUP);

  triTone();
  penUp();

  // NEU: Timer-basierte Motor-Steuerung initialisieren
  stepper_timer_init();
  stepper_timer_start();
  
  // Rampe aktivieren: 200 Schritte für Beschleunigung/Verzögerung
  stepper_set_ramp(200);
  
  ESP_LOGI(TAG, "GPTimer Motor-Steuerung initialisiert (mit Rampen)");
}

//=========================================================================== LOOP
void loop() {
  // TEST: Rampen-Demo mit Hardware-Timer
  ESP_LOGI(TAG, "=== GPTimer Rampen-Demo ===");
  
  // Startgeschwindigkeit setzen (langsam)
  stepper_set_speed(5000);  // 5000 µs = 200 Steps/s (Start)
  
  // Vorwärts starten
  ESP_LOGI(TAG, "Vorwärts mit Beschleunigung...");
  stepper_set_command(MotorCommand::FORWARD);
  
  // Beschleunigung zu schnell
  stepper_set_target_speed(1000);  // Rampe zu 1000 µs = 1000 Steps/s
  
  // Warten bis Rampe fertig + fahren
  while (stepper_is_ramping()) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  ESP_LOGI(TAG, "Volle Geschwindigkeit erreicht!");
  vTaskDelay(pdMS_TO_TICKS(3000));
  
  // Sanft stoppen (mit Verzögerungsrampe)
  ESP_LOGI(TAG, "Sanftes Stoppen...");
  stepper_smooth_stop();
  
  // Warten bis gestoppt
  while (stepper_is_ramping()) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  ESP_LOGI(TAG, "Gestoppt! Schritte: %ld", stepper_get_step_count());
  vTaskDelay(pdMS_TO_TICKS(2000));
  
  // Drehung mit Rampe
  ESP_LOGI(TAG, "Drehung CW mit Rampe...");
  stepper_set_speed(4000);  // Langsam starten
  stepper_set_command(MotorCommand::SPIN_CW);
  stepper_set_target_speed(800);  // Sehr schnell werden
  
  while (stepper_is_ramping()) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  ESP_LOGI(TAG, "Max Drehgeschwindigkeit!");
  vTaskDelay(pdMS_TO_TICKS(3000));
  
  // Abbremsen auf mittlere Geschwindigkeit
  ESP_LOGI(TAG, "Abbremsen...");
  stepper_set_target_speed(2500);
  
  while (stepper_is_ramping()) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  vTaskDelay(pdMS_TO_TICKS(2000));
  
  // Sanft stoppen
  stepper_smooth_stop();
  while (stepper_is_ramping()) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  
  stepper_reset_step_count();
  ESP_LOGI(TAG, "Zyklus beendet, Neustart in 3 Sek...");
  vTaskDelay(pdMS_TO_TICKS(3000));

  /*
  // ORIGINAL CODE - auskommentiert für Test
  // write "HELLO WORLD!" in 15mm high characters:
  plotText("Hello World! ", 15);
  */
}
