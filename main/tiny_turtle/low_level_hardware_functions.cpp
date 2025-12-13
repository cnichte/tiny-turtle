#include "low_level_hardware_functions.h"

#include "arduino_compat.h"
#include "constants.h"
#include "light_sound.h"
#include "tiny_turtle.h"
#include "esp_attr.h"  // für IRAM_ATTR
#include "driver/gpio.h"  // für gpio_set_level

//---------------------------------------------------------------------------- switch the stepper coils
// IRAM_ATTR: Funktion wird im RAM gehalten für schnellen ISR-Zugriff
static const DRAM_ATTR uint8_t stepperPins[2][4] = {  // stepper coil pins
  { STEP_1A, STEP_1B, STEP_1C, STEP_1D },
  { STEP_2A, STEP_2B, STEP_2C, STEP_2D }
};

static const DRAM_ATTR uint8_t halfStepPatterns[8][4] = {
  { 1, 0, 0, 0 },
  { 1, 1, 0, 0 },
  { 0, 1, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 1 },
  { 0, 0, 0, 1 },
  { 1, 0, 0, 1 }
};

// ISR-safe Schrittmotor-Steuerung (IRAM_ATTR in Header deklariert)
void switchStepper(uint8_t stepper, int direction) {
  uint8_t stepperIndex = stepper - 1;

  if (stepper == 1) {
    stepCount1 = (8 + direction + stepCount1) % 8;  // global state of step position
    const uint8_t* pattern = halfStepPatterns[stepCount1];
    for (int i = 0; i < 4; i++) {
      gpio_set_level(static_cast<gpio_num_t>(stepperPins[stepperIndex][i]), pattern[i]);
    }
  } else {
    stepCount2 = (8 + direction + stepCount2) % 8;  // global state of step position
    const uint8_t* pattern = halfStepPatterns[stepCount2];
    for (int i = 0; i < 4; i++) {
      gpio_set_level(static_cast<gpio_num_t>(stepperPins[stepperIndex][i]), pattern[i]);
    }
  }
}

//---------------------------------------------------------------------------- unpower stepper coils
// IRAM_ATTR in Header deklariert
void stopSteppers() {
  for (int i = 0; i < 4; i++) {
    gpio_set_level(static_cast<gpio_num_t>(stepperPins[0][i]), 0);
    gpio_set_level(static_cast<gpio_num_t>(stepperPins[1][i]), 0);
  }
}



//---------------------------------------------------------------------------- pen up and down (servo)
void penUp() {
  stopSteppers();
  led(15, 15, 15);
  if (drawing) {
    myservo.attach(SERVO);
    myservo.write(servoPenDown);
    delay(400);
    myservo.detach();
  }
  drawing = false;  // global state of pen position
}

void penDown() {
  stopSteppers();
  led(50, 50, 50);
  if (!drawing) {
    myservo.attach(SERVO);
    myservo.write(servoPenUp);
    delay(400);
    myservo.detach();
  }
  drawing = true;  // global state of pen position
}

//---------------------------------------------------------------------------- check both bumper switches and return true if there's a collision
bool checkBumper() {
  return !digitalRead(SWITCH_F) || !digitalRead(SWITCH_B);
}

//---------------------------------------------------------------------------- photo sensor check --> returns true if there's sth. dark detected
bool checkSensor() {
  return analogRead(SENSOR) < 512;
}
