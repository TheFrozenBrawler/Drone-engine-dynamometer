#include "sensors.h"

#include <Arduino.h>
#include "globals.h"
#include "functions.h"

float anemometer_measure(){
  int adcValue = analogRead(analogPin);             // ADC reading (0–1023)
  float voltage = (adcValue * vRef) / 1023.0;       // Recount for voltage 0–5V
  float scaleFactor = 5.0;                          // Scale factor - Vorg_max / Vdiv_max = 25/5 [V]
  float scaledVoltage = voltage * scaleFactor;      // recount for scale 0–25V

  float filtered_value = avg_filter(scaledVoltage);

  float air_speed = a_slope * filtered_value;

  
  return air_speed;
}
