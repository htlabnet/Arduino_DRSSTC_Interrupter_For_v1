#ifndef LIB_OUTPUT_H
#define LIB_OUTPUT_H

#include <avr/io.h>
#include <Arduino.h>

extern volatile bool beep_active;

void output_init();
void output_single_pulse(uint8_t pin, uint16_t ontime);
void output_dual_pulse(uint16_t ontime);

#endif
