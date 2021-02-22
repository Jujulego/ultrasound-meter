#include <Arduino.h>

#include "ultrasonic.hpp"

// Intern
volatile unsigned long lastUp = 0;
volatile unsigned long lastDown = 0;

void fallingInterrupt();

void risingInterrupt() {
  lastUp = micros();

  attachInterrupt(digitalPinToInterrupt(2), fallingInterrupt, FALLING);
}

void fallingInterrupt() {
  lastDown = micros();
  
  attachInterrupt(digitalPinToInterrupt(2), risingInterrupt, RISING);
}

// Constructor
Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
}

// Methods
void Ultrasonic::setup() const {
  // Setup pins
  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT_PULLUP);

  // Setup interrupts
  attachInterrupt(digitalPinToInterrupt(this->echoPin), risingInterrupt, RISING);
}

void Ultrasonic::trigger() const {
  digitalWrite(this->trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->trigPin, LOW);
}

uint64_t Ultrasonic::getMeasure() const {
  Serial.print(lastDown);
  Serial.print(" - ");
  Serial.println(lastUp);
  return (lastDown - lastUp) / 56;
}