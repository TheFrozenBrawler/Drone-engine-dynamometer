// Works only on arduino Uno R4 wifi or other board with Renesas RA4M1 chip

#include <FspTimer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_MLX90614.h>
#include "HX711.h"  // install HX711.h by Rob Tillart

#include "globals.h"
#include "logger.h"
#include "functions.h"
#include "sensors.h"

// Init of main variables
Adafruit_AHTX0 aht20;                           // temperature sensor
Adafruit_MLX90614 mlxA;// = Adafruit_MLX90614();   // IR temperature sensor, object A
Adafruit_MLX90614 mlxB;// = Adafruit_MLX90614();   // IR temperature sensor, object B
HX711 tensometer;

// Timer interrupt flag
volatile bool perform_measure = false;          

void setup() {
  Serial.begin(BAUD_RATE);
  // Serial.println("[INFO] Initialization of dynamometer");
  LOG_I("Initialization of dynamometer");

  // I2C setup
  Wire.begin();

  // Test sensors connection
  connect_sensors(&aht20, &mlxA, &mlxB);

  // Timer setup
  if (!timer_setup(SAMPLING)) {
    LOG_E("Timer initialization fault!");
    while(1);
  }
}

void loop() {
  if (perform_measure == true) {
      perform_measure = false;
      measure(&aht20, &mlxA, &mlxB, &tensometer);
  }
}
