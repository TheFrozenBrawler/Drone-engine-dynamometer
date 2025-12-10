#ifndef SENSORS_H
#define SENSORS_H

// #include <Adafruit_AHTX0.h>
// #include <Adafruit_MLX90614.h>

class Adafruit_AHTX0;
class Adafruit_MLX90614;
class Adafruit_HX711;
class Adafruit_INA228;

// Function to measure value of anemometer
float anemometer_measure();

// Function to measure temperature with AHT20
float aht20_temperature_measure(Adafruit_AHTX0 *aht20);

// Function to measure temperature wit MLX90614
float mlx_temperature_measure(Adafruit_MLX90614 *mlx_ptr);

// Function to measure tensometer value
float tensometer_measure(Adafruit_HX711 *tensometer);

// Functions to measure voltage, current, charge and temperature from power sensr
float power_current_measure(Adafruit_INA228 *pwr_snsr);
float power_voltage_measure(Adafruit_INA228 *pwr_snsr);
float power_charge_measure(Adafruit_INA228 *pwr_snsr);
float power_temperature_measure(Adafruit_INA228 *pwr_snsr);

#endif // SENSORS_H
