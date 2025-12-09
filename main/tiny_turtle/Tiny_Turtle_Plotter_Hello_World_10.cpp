#include <cmath>
#include <string>

#include "arduino_compat.h"
#include "constants.h"
#include "coordinates.h"
#include "esp_log.h"
#include "neopixel_compat.h"
#include "tiny_turtle.h"


static const char* TAG = "tiny_turtle";

NeoPixel pixels(NEOPIXEL, 1);

Servo myservo;           //                                                   servo to lift / lower the pen
uint8_t stepCount1 = 0;  //                                                   global sep counter motor 1
uint8_t stepCount2 = 0;  //                                                   global sep counter motor 2

long delayValue = 2000;  //                                                   current delay between steps
int direction = 1;       //                                                   current moving direction
bool drawing = true;     //                                                    pen position (true = pen is down / false = pen is up)

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
}

//=========================================================================== LOOP
void loop() {

  // write "HELLO WORLD!" in 15mm high characters:
  plotText("Hello World! ", 15);


  // draw a fish:
  // penUp();move(300);drawCoordinates(1.0);penUp();move(200);



  // draw a 10cm square:
  /* penDown();
  for (int i = 0; i < 4; i++) {
    move(100);
    turn(90);
  }
  penUp();
  move(50);
  */

}
