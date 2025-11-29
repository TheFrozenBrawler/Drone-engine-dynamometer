#include "sensors.h"
#include <Arduino.h>
#include <Adafruit_AHTX0.h>

#include "globals.h"
#include "functions.h"

////////// Anemometer //////////
// Main function to measure air speed 
static float anem_buffer[ANEM_WIN];
static int anem_index = 0;
static float anem_sum = 0.0;

float anemometer_measure(){
  int adc_value = analogRead(ANEMOMETER_PIN);             // ADC reading (0–1023)
  float raw_voltage = (adc_value * ADC_REF_V) / 1023.0;       // Recount for voltage 0–5V
  float scaled_voltage = raw_voltage * SCALE_FAC;      // recount for scale 0–25V

  float filtered_value = avg_filter(scaled_voltage, ANEM_WIN, anem_buffer, &anem_index, &anem_sum);

  float air_speed = ANEM_A_SLOPE * filtered_value;

  
  return air_speed;
}

////////// Temperature sensor //////////
// Main function to measure temperature
static float temp_buffer[TEMP_WIN];
static int temp_index = 0;
static float temp_sum = 0.0;

float temperature_measure(Adafruit_AHTX0 *aht20) {
  sensors_event_t humidity_unused, temp_aht;
  aht20->getEvent(&humidity_unused, &temp_aht);
  float raw_temperature = temp_aht.temperature;
  
  // average filter
  float temperature = avg_filter(raw_temperature, TEMP_WIN, temp_buffer, &temp_index, &temp_sum);

  return temperature;
}
