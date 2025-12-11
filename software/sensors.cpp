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
static float temp_ir_a_buffer[TEMP_IR_WIN];
static int temp_ir_a_index = 0;
static float temp_ir_a_sum = 0.0;

static float temp_ir_b_buffer[TEMP_IR_WIN];
static int temp_ir_b_index = 0;
static float temp_ir_b_sum = 0.0;

/**
* @brief Return measured object temperature of one MLX90614 sensor in Celsius degrees.
*
* @param mlx_ptr Pointer to object of Adafruit_MLX90614 sensor class
*
* @return Measured floating point value.
*/
float mlx_temperature_measure_A(Adafruit_MLX90614 *mlx_ptr) {
  float temperature = mlx_ptr->readObjectTempC();
  float temperature_avg = avg_filter(temperature, TEMP_IR_WIN, temp_ir_a_buffer, &temp_ir_a_index, &temp_ir_a_sum);

  return temperature_avg;
}

/**
* @brief Return measured object temperature of one MLX90614 sensor in Celsius degrees.
*
* @param mlx_ptr Pointer to object of Adafruit_MLX90614 sensor class
*
* @return Measured floating point value.
*/
float mlx_temperature_measure_B(Adafruit_MLX90614 *mlx_ptr) {
  float temperature = mlx_ptr->readObjectTempC();
  float temperature_avg = avg_filter(temperature, TEMP_IR_WIN, temp_ir_b_buffer, &temp_ir_b_index, &temp_ir_b_sum);

  return temperature_avg;
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
static float p_sens_cur_buffer[P_SENS_WIN];
static int p_sens_cur_index = 0;
static float p_sens_cur_sum = 0.0;

static float p_sens_v_buffer[P_SENS_WIN];
static int p_sens_v_index = 0;
static float p_sens_v_sum = 0.0;

/**
* @brief Perform measurements of current from power supply
*
* @return Measured current value of power supply, floating point
*/
float power_current_measure(Adafruit_INA228 *pwr_snsr) {
  float pwr_snsr_current_raw = pwr_snsr->getCurrent_mA();
  float pwr_snsr_current_avg = avg_filter(pwr_snsr_current_raw, P_SENS_WIN, p_sens_cur_buffer, &p_sens_cur_index, &p_sens_cur_sum);

  return pwr_snsr_current_avg;
}

/**
* @brief Perform measurements of voltage on power supply
*
* @return Measured voltage value of power supply, floating point
*/
float power_voltage_measure(Adafruit_INA228 *pwr_snsr) {
  float pwr_snsr_voltage_raw = pwr_snsr->getBusVoltage_V();
  float pwr_snsr_voltage_avg = avg_filter(pwr_snsr_voltage_raw, P_SENS_WIN, p_sens_v_buffer, &p_sens_v_index, &p_sens_v_sum);

  return pwr_snsr_voltage_avg;
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
