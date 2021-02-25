#pragma once

#include <Arduino.h>
#include <stdint.h>

// Constants
#define SEG_A   0x01
#define SEG_B   0x02
#define SEG_C   0x04
#define SEG_D   0x08
#define SEG_E   0x10
#define SEG_F   0x20
#define SEG_G   0x40
#define SEG_DOT 0x80

const uint8_t SEGMENTS_TABLE[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

// Class
template<uint8_t N>
class SegmentsDisplay {
  private:
    // Attributes
    uint8_t m_latchPin;
    uint8_t m_csPin;
    uint8_t m_dataPin;
    uint8_t m_digitsPin[N];

    uint8_t m_digits[N];

    // Methods
    void send(uint8_t data, uint8_t display) const {
      // Send data to registry
      digitalWrite(m_latchPin, LOW);
      shiftOut(m_dataPin, m_csPin, MSBFIRST, data);
      digitalWrite(m_latchPin, HIGH);

      // Select screen to update
      for (const uint8_t& pin: m_digitsPin) {
        digitalWrite(pin, (pin == display) ? LOW : HIGH);
      }
    }

  public:
    // Constructor
    SegmentsDisplay(uint8_t latch, uint8_t cs, uint8_t data, uint8_t digits[N])
    : m_latchPin(latch), m_csPin(cs), m_dataPin(data) {
      for (uint8_t i = 0; i < N; ++i) {
        m_digitsPin[i] = digits[i];
        m_digits[i] = 0;
      }
    }

    // Methods
    void setup() const {
      pinMode(m_latchPin, OUTPUT);
      pinMode(m_csPin, OUTPUT);
      pinMode(m_dataPin, OUTPUT);

      for (const uint8_t& pin: m_digitsPin) {
        pinMode(pin, OUTPUT);
      }
    }

    void process() const {
      // Print all digits
      for (uint8_t i = 0; i < N; ++i) {
        send(m_digits[i], m_digitsPin[i]);
        delayMicroseconds(200);
      }
    }

    void print(unsigned long num) {
      for (uint8_t i = N; i > 0; --i) {
        m_digits[i-1] = SEGMENTS_TABLE[num % 10];
        num /= 10;
      }
    }

    void print(double num, uint8_t precision = 0) {
      if (precision > 0 && precision < N) {
        num *= pow(10, precision);
      }

      print((unsigned long) num);

      if (precision > 0 && precision < N) {
        m_digits[N - precision - 1] |= SEG_DOT;
      }
    }
};