#include "globals.h"

#include <Arduino.h>


// connection variables
const int baud_rate  = 9600;

// measure variables
const int analogPin = A0;       // analog output pin
const float vRef = 5.0;         // reference voltage
const float scaleFactor = 5.0;  // scale: 0-5V measured = 0-25V real → times 5
const short SAMPLING = 100;     // milliseconds 

// average filter variables
//const int win = 10;

// anemometer measure variables
const float a_slope = 2.8; // for linear function
