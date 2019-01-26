#ifndef LIB_INPUT_H
#define LIB_INPUT_H

#include <avr/io.h>
#include <Arduino.h>
#include "settings.h"

extern volatile unsigned int adc_vr_1;
extern volatile unsigned int adc_vr_2;
extern volatile unsigned int adc_vr_3;
extern volatile unsigned int adc_vr_4;
extern volatile boolean gpio_sw_1;
extern volatile boolean gpio_sw_2;

void input_init();
void input_task();
byte menu_select();

#endif
