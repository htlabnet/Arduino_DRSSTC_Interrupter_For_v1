#ifndef LIB_OSC_H
#define LIB_OSC_H

#include <avr/io.h>
#include <Arduino.h>
#include "settings.h"

void osc_timer_init();
void osc_timer_init_64();
void osc_timer_enable(uint8_t osc, uint16_t period);
void osc_timer_disable(uint8_t osc);
void osc_timer_set(uint8_t osc, uint16_t period);

#endif
