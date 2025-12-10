#include "sensors.h"
#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_HX711.h>
#include <Adafruit_INA228.h>

#include "globals.h"
#include "functions.h"

////////// Anemometer //////////
static float anem_buffer[ANEM_WIN];
static int anem_index = 0;
static float anem_sum = 0.0;

/**
* @brief Measure air speed
*
* @return Floating point value of measured air speed
*/
float anemometer_measure() {
  int adc_value = analogRead(ANEMOMETER_PIN);             // ADC reading (0–1023)
  float raw_voltage = (adc_value * ADC_REF_V) / 1023.0;       // Recount for voltage 0–5V
  float scaled_voltage = raw_voltage * SCALE_FAC;      // recount for scale 0–25V

  float filtered_value = avg_filter(scaled_voltage, ANEM_WIN, anem_buffer, &anem_index, &anem_sum);

  float air_speed = ANEM_A_SLOPE * filtered_value;

  return air_speed;
}

////////// Temperature sensor //////////
static float temp_buffer[TEMP_WIN];
static int temp_index = 0;
static float temp_sum = 0.0;

/**
* @brief Measure temperature with regular sensor, AHT20
*
* @param aht20 Pointer to object of Adafruit_AHTX0 sensor class
*
* @return Floating point value of measured temperature
*/
float aht20_temperature_measure(Adafruit_AHTX0 *aht20) {
  sensors_event_t humidity_unused, temp_aht;
  aht20->getEvent(&humidity_unused, &temp_aht);
  float raw_temperature = temp_aht.temperature;
  
  // average filter
  float temperature = avg_filter(raw_temperature, TEMP_WIN, temp_buffer, &temp_index, &temp_sum);

  return temperature;
}

////////// IR temperature sensor //////////
/**
* @brief Return measured object temperature of one MLX90614 sensor in Celsius degrees.
*
* @param mlx_ptr Pointer to object of Adafruit_MLX90614 sensor class
*
* @return Measured floating point value.
*/
float mlx_temperature_measure(Adafruit_MLX90614 *mlx_ptr) {
  return mlx_ptr->readObjectTempC();
}

////////// Tensometer //////////
/**
* @brief Perform tensometer measurement
*
* @param tensometer Pointer to object of Adafruit_HX711.h sensor class
*
* @return Measured floating point value
*/
float tensometer_measure(Adafruit_HX711 *tensometer) {
  int32_t tensometer_val_raw = tensometer->readChannelBlocking(CHAN_A_GAIN_128);
  float tensometer_val_scaled = (double)tensometer_val_raw / 100.0;

  return tensometer_val_scaled;
}

////////// Power Sensor //////////
/**
* @brief Perform measurements of current from power supply
*
* @return Measured current value of power supply, floating point
*/
float power_current_measure(Adafruit_INA228 *pwr_snsr) {
  return pwr_snsr->getCurrent_mA();
}

/**
* @brief Perform measurements of voltage on power supply
*
* @return Measured voltage value of power supply, floating point
*/
float power_voltage_measure(Adafruit_INA228 *pwr_snsr) {
  return pwr_snsr->getBusVoltage_V();
}

/**
* @brief Perform measurements of charge from power supply
*
* @return Measured charge value of power supply, floating point
*/
float power_charge_measure(Adafruit_INA228 *pwr_snsr) {
  return pwr_snsr->readCharge();
}

/**
* @brief Perform measurements of temperature of sensor
*
* @return Measured temperature of sensor, floating point
*/
float power_temperature_measure(Adafruit_INA228 *pwr_snsr) {
  return pwr_snsr->readDieTemp();
}
