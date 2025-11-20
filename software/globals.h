#ifndef GLOBALS_H
#define GLOBALS_H

// general
#define BAUD_RATE 115200

// measure variables
#define ANEMOMETER_PIN A0       // analog output pin
#define ADC_REF_V 5.0         // reference voltage
#define SCALE_FAC 5.0  // scale: 0-5V measured = 0-25V real → times 5
#define SAMPLING 100     // measure sampling, milliseconds 

// average filter variables
#define TEMP_WIN 5
#define ANEM_WIN 10

// anemometer measure variables
#define ANEM_A_SLOPE 2.8 // for linear function


#endif // GLOBALS_H
