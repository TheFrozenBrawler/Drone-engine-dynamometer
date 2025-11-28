#include "functions.h"

#include <Arduino.h>
#include <FspTimer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

#include "globals.h"
#include "sensors.h"

////////// SENSOR HELP FUNCTIONS //////////
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

// connect and test AHT20 temperature sensor
void connect_AHT20(Adafruit_AHTX0 *aht20) {
  if (!aht20->begin()) {
    Serial.println(F("[ERROR] AHT20 not detected!"));
    while (1) delay(10);
  } else {
    Serial.println(F("[INFO] AHT20 OK"));
  }
}

////////// MEASURE //////////
// main measure function
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

////////// TIMERS ////////// 
// Timer interrupt function
extern bool perform_measure;
void onTimerCallback(timer_callback_args_t *args) {
  perform_measure = true;
}

// Timer setup
FspTimer timer;
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