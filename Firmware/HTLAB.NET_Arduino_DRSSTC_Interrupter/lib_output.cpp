#include "lib_output.h"

void output_init() {
  
  // Pin Settings
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
}


void output_single_pulse(byte pin, unsigned int ontime) {
  
  // Arduino Uno (ATmega328)
  // D8-9 = PB0-1
  #if defined(ARDUINO_AVR_UNO)
    PORTB |= _BV(pin);
    delayMicroseconds(ontime + 1);
    PORTB &= ~_BV(pin);
  
  // Arduino Leonardo, Micro (ATmega32u4)
  // D8-9 = PB4-5
  #elif (defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO))
    PORTB |= _BV(pin + 4);
    delayMicroseconds(ontime + 1);
    PORTB &= ~_BV(pin + 4);
  
  #endif
  
}


