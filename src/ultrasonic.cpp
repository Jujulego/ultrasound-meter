#include <Arduino.h>
#include <stdint.h>

#include "ultrasonic.hpp"

#define MEAN_WEIGHT 0.7

// Intern
uint8_t echoPin = 0;
uint8_t trigPin = 0;

volatile uint64_t lastUp = 0UL;
volatile uint64_t measure = 0UL;
volatile uint64_t mean = 0UL;

// Interrupts
void risingInterrupt();
void fallingInterrupt();

void risingInterrupt() {
  lastUp = micros();
  attachInterrupt(digitalPinToInterrupt(echoPin), fallingInterrupt, FALLING);
}

void fallingInterrupt() {
  measure = lastUp - micros();
  mean = mean * (1 - MEAN_WEIGHT) + measure * MEAN_WEIGHT;
  attachInterrupt(digitalPinToInterrupt(echoPin), risingInterrupt, RISING);
}

// Statics
void Ultrasonic::setup(uint8_t _echoPin, uint8_t _trigPin) {
  // Store and initiate vars
  echoPin = _echoPin;
  trigPin = _trigPin;
  measure = 0UL;

  // Setup pins and 1st interrupt
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);

  attachInterrupt(digitalPinToInterrupt(echoPin), risingInterrupt, RISING);
}

// Methods
void Ultrasonic::trigger() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

double Ultrasonic::getRawMeasure() const {
  return measure / 56.0;
}

double Ultrasonic::getMeasure() {
  return mean / 56.0;
}