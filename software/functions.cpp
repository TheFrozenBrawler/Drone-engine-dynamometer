#include "functions.h"

#include <Arduino.h>
#include <FspTimer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_HX711.h>

#include "globals.h"
#include "logger.h"
#include "sensors.h"

////////// SENSOR HELP FUNCTIONS //////////
/**
* @brief Perform filtration on given samples with average filter
*
* @param sample The sample that has been currently measured
* @param win The filter window, average of how many samples will be calculated
* @param buffer Pointer to buffer with samples
* @param index The index of current sample inside buffer
* @param sum Sum of all samples inside buffer   
*/
float avg_filter(float sample, int win, float *buffer, int *index, float *sum) {  
  *sum -= buffer[*index];
  buffer[*index] = sample;
  *sum += buffer[*index];
  float average = *sum / win;
  *index += 1;
  (*index == win) ? *index = 0 : *index;

  return average;
}
/**
 * @brief Connect and test sensors
 * 
 * @param aht20 Pointer to AHT20 sensor's class object
 * @param mlxA Pointer to MLX A sensor's class object
 * @param mlxB Pointer to MLX B sensor's class object
 */
void connect_sensors(Adafruit_AHTX0 *aht20, Adafruit_MLX90614 *mlxA, Adafruit_MLX90614 *mlxB, Adafruit_HX711 *tensometer) {
  // Connect and check AHT20
  if (!aht20->begin()) {
    LOG_E("AHT20 not detected!");
    while (1) delay(10);
  } else {
    LOG_I("AHT20 OK");
  }

  // Connect and check MLXA
  if (!mlxA->begin(MLXA_ADDR)) {
    LOG_E("MLX A not detected!");
    while (1) delay(10);
  } else {
    LOG_I("MLX A OK");

  }

  // Connect and check MLXB
  if (!mlxB->begin(MLXB_ADDR)) {
    LOG_E("MLX B not detected!");
    while (1) delay(10);
  } else {
    LOG_I("MLX B OK");
  }

  // Connect, calibrate and check tensometer
  tensometer->begin();
  Serial.println("\n\TENSOMETER CALIBRATION\n===========");
  Serial.println("remove all weight from the loadcell");
  //  flush Serial input
  while (Serial.available()) Serial.read();
  Serial.println("and press enter\n");
  while (Serial.available() == 0);
  Serial.println("Tareing...");
  for (uint8_t t=0; t<3; t++) {
    tensometer->tareA(tensometer->readChannelRaw(CHAN_A_GAIN_128));
    tensometer->tareA(tensometer->readChannelRaw(CHAN_A_GAIN_128));
  }
}

////////// MEASURE //////////
/**
 * @brief Main function to perform all the measurements
 * 
 * @param aht20 Pointer to AHT20 sensor's class object
 * @param mlxA Pointer to MLX A sensor's class object
 * @param mlxB Pointer to MLX B sensor's class object
 * @param tensometer Pointer to tensometer's class object
 */
bool measure(Adafruit_AHTX0 *aht20_ptr,
             Adafruit_MLX90614 *mlxA_ptr, 
             Adafruit_MLX90614 *mlxB_ptr,
             Adafruit_HX711 *tensometer) {
  float air_speed = anemometer_measure();
  float T_AHT20 = aht20_temperature_measure(aht20_ptr);
  float T_MLXA = mlx_temperature_measure(mlxA_ptr);
  float T_MLXB = mlx_temperature_measure(mlxB_ptr);
  float tensometer_val = tensometer_measure(tensometer);

  LOG_TAG_M();

  LOG_M("Air speed: ");
  SEND_MEAS(air_speed, 2);

  LOG_M(" Temperature AHT20: ");
  SEND_MEAS(T_AHT20, 2);

  LOG_M(" Temperature MLX A: ");
  SEND_MEAS(T_MLXA, 2);

  LOG_M(" Temperature MLX B: ");
  SEND_MEAS(T_MLXB, 2);

  LOG_M(" Tensometer: ");
  SEND_MEAS(tensometer_val, 2);

  SEND_ENDLN();

  return true;
}

////////// TIMERS ////////// 
/**
 * @brief Timer interrupt function that rises flag to perform measurements
 * 
 * @param args Pointer to pass arguments - UNUSED
 */
extern bool perform_measure;
void onTimerCallback(timer_callback_args_t *args) {
  perform_measure = true;
}

FspTimer timer;
/**
 * @brief Function to setup measurement timer
 * 
 * @param tim_period_ms Variable to pass timer period value, in miliseconds
 */
bool timer_setup(uint16_t tim_period_ms) {
  uint8_t timer_type = GPT_TIMER;
  float tim_freq = 1 / ((float)tim_period_ms / 1000);
  int8_t tindex = FspTimer::get_available_timer(timer_type);
  if (tindex < 0){
    tindex = FspTimer::get_available_timer(timer_type, true);
  }
  if (tindex < 0){
    return false;
  }

  FspTimer::force_use_of_pwm_reserved_timer();

  if(!timer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, tim_freq, 0.0f, onTimerCallback)){
    return false;
  }

  if (!timer.setup_overflow_irq()){
    return false;
  }

  if (!timer.open()){
    return false;
  }

  if (!timer.start()){
    return false;
  }
  return true;
}