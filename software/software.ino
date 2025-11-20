// for arduino mega
#include <arduino-timer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

#include "globals.h"
#include "functions.h"
#include "sensors.h"


auto timer = timer_create_default(); 
Adafruit_AHTX0 aht20;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("[INFO] Initialization of dynamometer");

  // I2C setup
  Wire.begin();

  // test sensors
  connect_AHT20(&aht20);

  timer.every(SAMPLING, measure, &aht20);
}

void loop() {
  timer.tick();
}
