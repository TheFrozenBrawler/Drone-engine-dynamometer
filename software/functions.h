#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Wire.h>

class Adafruit_AHTX0;
class Adafruit_MLX90614;
class Adafruit_HX711;

// Average filter
float avg_filter(float sample, int win, float *buffer, int *index, float *sum);

// Function to test AHT20 temperature sensor
void connect_sensors(Adafruit_AHTX0 *aht20, Adafruit_MLX90614 *mlxA, Adafruit_MLX90614 *mlxB, Adafruit_HX711 *tensometer);

// Main measure functon to start all sensors
bool measure(Adafruit_AHTX0 *aht20_ptr, Adafruit_MLX90614 *mlxA_ptr, Adafruit_MLX90614 *mlxB_ptr, Adafruit_HX711 *tensometer);

// Timer setup
bool timer_setup(uint16_t tim_period_ms);

#endif //FUNCTIONS_H
