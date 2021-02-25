#include <Arduino.h>

#include "segments.hpp"
#include "ultrasonic.hpp"

#define NUM_OF_DIGITS 4
#define DOT 0x80

unsigned long prevDisplay = 0, deltaDisplay = 500;
unsigned long prevUltrasonic = 0, deltaUltrasonic = 500;

Ultrasonic ultrasonic;

SegmentsDisplay<4> segments(4, 5, 3, (uint8_t[]) {11, 10, 9, 8});

void setup() {
  Serial.begin(9600);
  
  segments.setup();
  Ultrasonic::setup(2, 6);
}

void loop() {
  segments.process();

  unsigned long t = millis();
  if (t > prevDisplay + deltaDisplay) {
    prevDisplay = t;

    double m = ultrasonic.getMeasure();
    segments.print(m, 1);
  }

  if (t > prevUltrasonic + deltaUltrasonic) {
    prevUltrasonic = t;
    ultrasonic.trigger();
  }
}