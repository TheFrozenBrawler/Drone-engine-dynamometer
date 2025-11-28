// Works only on arduino Uno R4 wifi or other board with Renesas RA4M1 chip

#include <FspTimer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

#include "globals.h"
#include "functions.h"
#include "sensors.h"


Adafruit_AHTX0 aht20;
volatile bool perform_measure = false;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("[INFO] Initialization of dynamometer");

  // I2C setup
  Wire.begin();

  // Test sensors
  connect_AHT20(&aht20);

  // Timer setup
  if (!timer_setup(SAMPLING)) {
    Serial.println("[ERROR] Tmer initialization fault!");
    while(1);
  }
}

void loop() {
  if (perform_measure == true) {
      perform_measure = false;
      measure(&aht20);
  }
}
