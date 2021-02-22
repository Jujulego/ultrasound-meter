#pragma once

#include <stdint.h>

// Class
class Ultrasonic {
  private:
    // Attributes
    uint8_t trigPin;
    uint8_t echoPin;

  public:
    // Constructor
    Ultrasonic(uint8_t trigPin, uint8_t echoPin);

    // Methods
    void setup() const;
    void trigger() const;

    uint64_t getMeasure() const;
};