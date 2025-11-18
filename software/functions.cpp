#include "functions.h"

#include <Arduino.h>
#include "globals.h"

const int win = 10;
static float buffer[win];
static int index = 0;
static float sum = 0.0;

// average filter
float avg_filter(float sample) {  
  sum -= buffer[index];
  buffer[index] = sample;
  sum += buffer[index];
  float average = sum / win;
  index += 1;
  (index == win) ? index = 0 : index;

  return average;
}
