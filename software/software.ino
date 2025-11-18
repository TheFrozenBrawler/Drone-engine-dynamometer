// for arduino mega
#include <arduino-timer.h>
#include "globals.h"
#include "functions.h"
#include "sensors.h"


auto timer = timer_create_default(); 

void setup() {
  Serial.begin(baud_rate);

  timer.every(SAMPLING, anemometer_measure);
}

void loop() {
  timer.tick();
}
