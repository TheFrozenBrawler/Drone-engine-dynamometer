#ifndef GLOBALS_H
#define GLOBALS_H

// General
#define BAUD_RATE 115200
#define CSV_DIV ";"         // CSV divider

// Device mode (DEBUG or RUN)
// #define DEBUG   // comment this line to disable logging and enable RUN mode
#ifndef DEBUG
    #define RUN
#endif

// Measure global consts
#define ANEMOMETER_PIN A0       // analog output pin
#define ADC_REF_V 5.0         // reference voltage
#define SCALE_FAC 5.0  // scale: 0-5V measured = 0-25V real → times 5
#define SAMPLING 100     // measure sampling, milliseconds 

// Average filter consts
#define TEMP_WIN 5
#define ANEM_WIN 10

// Anemometer measure consts
#define ANEM_A_SLOPE 2.8 // for linear function

// Temperature IR sensors consts
#define MLXA_ADDR 0x5A
#define MLXB_ADDR 0x5B

// Tensometer setup
#define TENS_DT_PIN 6
#define TENS_SCK_PIN 7
#define TENS_OFFSET 163927
#define TENS_SETSCALE 112.587975

// Power sensor setup
#define PWR_SENS_ADDR 0x45

#endif // GLOBALS_H
