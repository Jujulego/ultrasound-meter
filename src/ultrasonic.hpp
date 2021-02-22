#pragma once

#include <stdint.h>

// Class
class Ultrasonic {
  public:
    // Constructor
    Ultrasonic() {};

    // Statics
    static void setup(uint8_t echoPin, uint8_t trigPin);

    // Methods
    void trigger();

    double getRawMeasure() const;
    double getMeasure();
};