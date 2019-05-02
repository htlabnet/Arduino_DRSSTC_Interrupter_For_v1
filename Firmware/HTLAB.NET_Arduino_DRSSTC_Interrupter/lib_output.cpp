#include "lib_output.h"

void output_init() {

  // Pin Settings
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

}


void output_single_pulse(uint8_t pin, uint16_t ontime) {

  // Arduino Leonardo, Micro (ATmega32u4)
  // D10-11 = PB6-7
  PORTB |= _BV(pin + 6);
  delayMicroseconds(ontime + 1);
  PORTB &= ~_BV(pin + 6);

}


void output_dual_pulse(uint16_t ontime) {

  // Arduino Leonardo, Micro (ATmega32u4)
  // D10-11 = PB6-7
  PORTB |= _BV(6) | _BV(7);
  delayMicroseconds(ontime + 1);
  PORTB &= ~(_BV(6) | _BV(7));

}
