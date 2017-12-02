#ifndef LIB_OUTPUT_H
#define LIB_OUTPUT_H

#include <avr/io.h>
#include <Arduino.h>

void output_init();
void output_single_pulse(byte pin, unsigned int ontime);

#endif


