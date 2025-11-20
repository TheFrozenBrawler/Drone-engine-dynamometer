#include "functions.h"

#include <Arduino.h>
#include <Wire.h>
#include <SoftWire.h>   
#include <Adafruit_AHTX0.h>

#include "globals.h"
#include "sensors.h"

// average filter
float avg_filter(float sample, int win, float *buffer, int *index, float *sum) {  
  *sum -= buffer[*index];
  buffer[*index] = sample;
  *sum += buffer[*index];
  float average = *sum / win;
  *index += 1;
  (*index == win) ? *index = 0 : *index;

  return average;
}

bool measure(void *aht20_ptr) {
  Adafruit_AHTX0* aht20 = (Adafruit_AHTX0*) aht20_ptr;
  float air_speed = anemometer_measure();
  float temperature = temperature_measure(aht20);

  Serial.print(F("[MEAS] Air speed: "));
  Serial.print(air_speed, 2);
  Serial.print(F("; Temperature: "));
  Serial.println(temperature, 2);

  return true;
}

void connect_AHT20(Adafruit_AHTX0 *aht20) {
  if (!aht20->begin()) {
    Serial.println(F("Błąd: nie wykryto AHT20!"));
    while (1) delay(10);
  } else {
    Serial.println(F("AHT20 OK"));
  }
}
