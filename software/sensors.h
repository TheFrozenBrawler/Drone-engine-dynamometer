#ifndef SENSORS_H
#define SENSORS_H

// #include <Adafruit_AHTX0.h>
// #include <Adafruit_MLX90614.h>

class Adafruit_AHTX0;
class Adafruit_MLX90614;

// Function to measure value of anemometer
float anemometer_measure();

// Function to measure temperature with AHT20
float aht20_temperature_measure(Adafruit_AHTX0 *aht20);

// Function to measure temperature wit MLX90614
float mlx_temperature_measure(Adafruit_MLX90614 *mlx_ptr);

#endif // SENSORS_H
