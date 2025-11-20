#ifndef GLOBALS_H
#define GLOBALS_H

// connection variables
extern const int baud_rate;

// measure variables
extern const int analogPin;       // analog output pin
extern const float vRef;         // reference voltage
extern const float scaleFactor;  // scale: 0-5V measured = 0-25V real → times 5
extern const short SAMPLING;     // milliseconds 

// average filter variables
//extern const int win;

// anemometer measure variables
extern const float a_slope; // for linear function

#endif // GLOBALS_H
