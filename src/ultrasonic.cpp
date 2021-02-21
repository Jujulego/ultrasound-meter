#include <Arduino.h>

#include "ultrasonic.hpp"

// Constructor
Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
}

// Methods
void Ultrasonic::setup() const {
  // Setup pins
  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT);

  // Setup interrupts
  attachInterrupt(digitalToInterrupt(this->echoPin), () => {
    this->lastUp = micros();
  }, RISING);

  attachInterrupt(digitalToInterrupt(this->echoPin), () => {
    this->lastMeasure = micros() - this->lastUp;
  }, FALLING);
}

void Ultrasonic::trigger() const {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
}

uint64_t Ultrasonic::getMeasure() const {
  return this->lastMeasure / 56;
}