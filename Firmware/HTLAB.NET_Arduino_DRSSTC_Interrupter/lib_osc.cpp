#include "lib_osc.h"

//extern void output_single_pulse(byte pin, unsigned int ontime);

void osc_timer_init() {
  noInterrupts();
  
  TCCR1A = 0;   // Set Timer1
  #if OSC_TIMER_DIVIDER == 8
    TCCR1B = _BV(WGM12) | _BV(CS11);    // Set CTC Mode (Prescaler=8)
  #elif OSC_TIMER_DIVIDER == 64
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);    // Set CTC Mode (Prescaler=64)
  #endif
  
  #if defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
    TCCR3A = 0;   // Set Timer3
    #if OSC_TIMER_DIVIDER == 8
      TCCR3B = _BV(WGM32) | _BV(CS31);    // Set CTC Mode (Prescaler=8)
    #elif OSC_TIMER_DIVIDER == 64
      TCCR3B = _BV(WGM32) | _BV(CS31) | _BV(CS30);    // Set CTC Mode (Prescaler=64)
    #endif
  #endif
  
  interrupts();
}


void osc_timer_enable(byte osc, unsigned int period) {
  switch(osc) {
    case 0:
      OCR1A = period;
      TCNT1 = 0;
      TIMSK1 |= _BV(OCIE1A);
      break;
    case 1:
      #if defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
        OCR3A = period;
        TCNT3 = 0;
        TIMSK3 |= _BV(OCIE3A);
        break;
      #endif
  }
}


void osc_timer_disable(byte osc) {
  switch(osc) {
    case 0:
      TIMSK1 &= ~_BV(OCIE1A);
      break;
    case 1:
      #if defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
        TIMSK3 &= ~_BV(OCIE3A);
        break;
      #endif
  }
}


