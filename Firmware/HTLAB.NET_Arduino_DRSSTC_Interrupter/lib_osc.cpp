#include "lib_osc.h"

void osc_timer_init() {
  noInterrupts();

  TCCR1A = 0;   // Set Timer1
  #if OSC_TIMER_DIVIDER == 8
    TCCR1B = _BV(WGM12) | _BV(CS11);    // Set CTC Mode (Prescaler=8)
  #elif OSC_TIMER_DIVIDER == 64
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);    // Set CTC Mode (Prescaler=64)
  #endif

  TCCR3A = 0;   // Set Timer3
  #if OSC_TIMER_DIVIDER == 8
    TCCR3B = _BV(WGM32) | _BV(CS31);    // Set CTC Mode (Prescaler=8)
  #elif OSC_TIMER_DIVIDER == 64
    TCCR3B = _BV(WGM32) | _BV(CS31) | _BV(CS30);    // Set CTC Mode (Prescaler=64)
  #endif

  interrupts();
}

void osc_timer_init_64() {
  noInterrupts();

  TCCR1A = 0;   // Set Timer1
  TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);    // Set CTC Mode (Prescaler=64)

  TCCR3A = 0;   // Set Timer3
  TCCR3B = _BV(WGM32) | _BV(CS31) | _BV(CS30);    // Set CTC Mode (Prescaler=64)

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
      OCR3A = period;
      TCNT3 = 0;
      TIMSK3 |= _BV(OCIE3A);
      break;
  }
}


void osc_timer_disable(byte osc) {
  switch(osc) {
    case 0:
      TIMSK1 &= ~_BV(OCIE1A);
      break;
    case 1:
      TIMSK3 &= ~_BV(OCIE3A);
      break;
  }
}


void osc_timer_set(byte osc, unsigned int period) {
  switch(osc) {
    case 0:
      OCR1A = period;
      if (TCNT1 >= period) {
        TCNT1 = period - 1;
      }
      break;
    case 1:
      OCR3A = period;
      if (TCNT3 >= period) {
        TCNT3 = period - 1;
      }
      break;
  }
}
