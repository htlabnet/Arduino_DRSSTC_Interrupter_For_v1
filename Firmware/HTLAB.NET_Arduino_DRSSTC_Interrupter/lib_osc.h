#ifndef LIB_OSC_H
#define LIB_OSC_H

#include <avr/io.h>
#include <Arduino.h>
#include "settings.h"

void osc_timer_init();
void osc_timer_init_64();
void osc_timer_enable(byte osc, unsigned int period);
void osc_timer_disable(byte osc);
void osc_timer_set(byte osc, unsigned int period);

#endif
