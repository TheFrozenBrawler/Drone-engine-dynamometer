// Works only on arduino Uno R4 wifi or other board with Renesas RA4M1 chip

#include <FspTimer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_MLX90614.h>
#include "Adafruit_HX711.h"
#include <Adafruit_INA228.h>


#include "globals.h"
#include "logger.h"
#include "functions.h"
#include "sensors.h"

// Init of main variables
Adafruit_AHTX0 aht20;                                 // temperature sensor
Adafruit_MLX90614 mlxA;                               // IR temperature sensor, object A
Adafruit_MLX90614 mlxB;                               // IR temperature sensor, object B
Adafruit_HX711 tensometer(TENS_DT_PIN, TENS_SCK_PIN); // Tensometer
Adafruit_INA228 pwr_snsr;                             // Supply power sensor

// Timer interrupt flag
volatile bool perform_measure = false;          

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.print("MODE "); 
  #ifdef DEBUG
    Serial.print("DEBUG");
  #else
    Serial.print("RUN");
  #endif
  LOG_I("Initialization of dynamometer");

  // I2C setup
  Wire.begin();

  // Test sensors connection
  connect_sensors(&aht20, &mlxA, &mlxB, &tensometer, &pwr_snsr);

  // Timer setup
  if (!timer_setup(SAMPLING)) {
    LOG_E("Timer initialization fault!");
    while(1);
  }
}

void loop() {
  if (perform_measure == true) {
      perform_measure = false;
      measure(&aht20, &mlxA, &mlxB, &tensometer, &pwr_snsr);
  }
}
