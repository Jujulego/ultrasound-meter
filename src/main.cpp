#include <Arduino.h>

#include "ultrasonic.hpp"

#define NUM_OF_DIGITS 4
#define DOT 0x80

int latch = 4; // 74HC595 pin 9  STCP
int cs = 5;    // 74HC595 pin 10 SHCP
int data = 3;  // 74HC595 pin 8  DS
int dPin[4] = {11, 10, 9, 8};

/*
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
*/

//  DP G F E D C B A
//0: 0 0 1 1 1 1 1 1 0x3f
//1: 0 0 0 0 0 1 1 0 0x06
//2: 0 1 0 1 1 0 1 1 0x5b
//3: 0 1 0 0 1 1 1 1 0x4f
//4: 0 1 1 0 0 1 1 0 0x66
//5: 0 1 1 0 1 1 0 1 0x6d
//6: 0 1 1 1 1 1 0 1 0x7d
//7: 0 0 0 0 0 1 1 1 0x07
//8: 0 1 1 1 1 1 1 1 0x7f
//9: 0 1 1 0 1 1 1 1 0x6f
unsigned char table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

int digits[NUM_OF_DIGITS] = {0, 0, 0, 0};

unsigned long prevDisplay = 0, deltaDisplay = 500;
unsigned long prevUltrasonic = 0, deltaUltrasonic = 500;
Ultrasonic ultrasonic;

void updateN();

void setup() {
  Serial.begin(9600);
  pinMode(latch, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(data, OUTPUT);

  for (const int& pin: dPin) {
    pinMode(pin, OUTPUT);
  }

  Ultrasonic::setup(2, 6);
}

void loop() {
  updateN();

  unsigned long t = millis();
  if (t > prevDisplay + deltaDisplay) {
    prevDisplay = t;

    long n = round(ultrasonic.getMeasure());
    for (int i = NUM_OF_DIGITS; i > 0; --i) {
      digits[i-1] = n % 10;
      n /= 10;
    }
  }

  if (t > prevUltrasonic + deltaUltrasonic) {
    prevUltrasonic = t;
    ultrasonic.trigger();
  }
}

void displayN(int i, int n, bool dot = false) {
  digitalWrite(latch, LOW);
  unsigned char code = table[n];
  if (dot) {
    code = code | DOT;
  }
  shiftOut(data, cs, MSBFIRST, code);
  digitalWrite(latch, HIGH);

  for (const int& d: dPin) {
    digitalWrite(d, HIGH);
  }
  digitalWrite(dPin[i], LOW);
}

void updateN() {
  for (int i = 0; i < NUM_OF_DIGITS; i++) {
    displayN(i, digits[i]);
    delay(5);
  }
}
