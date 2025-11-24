#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Wire.h>
#include <Adafruit_AHTX0.h>

// average filter
float avg_filter(float sample, int win, float *buffer, int *index, float *sum);

// function to test AHT20 temperature sensor
void connect_AHT20(Adafruit_AHTX0 *aht20);

// main measure functon to start all sensors
bool measure(void *aht20_ptr);

#endif //FUNCTIONS_H
