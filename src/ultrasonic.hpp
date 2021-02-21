#pragma once

#include <stdint.h>

// Class
class Ultrasonic {
  private:
    // Attributes
    uint8_t trigPin;
    uint8_t echoPin;

    volatile uint64_t lastUp = 0UL;
    volatile uint64_t lastMeasure = 0UL;

  public:
    // Constructor
    Ultrasonic(uint8_t trigPin, uint8_t echoPin);

    // Methods
    void setup() const;
    void trigger() const;

    uint64_t getMeasure() const;
};