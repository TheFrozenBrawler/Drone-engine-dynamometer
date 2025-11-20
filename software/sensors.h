#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_AHTX0.h>


// Function to measure value of anemometer
float anemometer_measure();

// Function to measure temperature
float temperature_measure(Adafruit_AHTX0 *aht20);

#endif // SENSORS_H
