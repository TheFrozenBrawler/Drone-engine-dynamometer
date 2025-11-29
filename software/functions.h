#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Wire.h>
#include <Adafruit_AHTX0.h>

// Average filter
float avg_filter(float sample, int win, float *buffer, int *index, float *sum);

// Function to test AHT20 temperature sensor
void connect_AHT20(Adafruit_AHTX0 *aht20);

// Main measure functon to start all sensors
bool measure(void *aht20_ptr);

// Timer setup
bool timer_setup(uint16_t tim_period_ms);

#endif //FUNCTIONS_H
