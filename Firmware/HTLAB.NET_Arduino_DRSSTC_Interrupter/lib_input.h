#ifndef LIB_INPUT_H
#define LIB_INPUT_H

#include <avr/io.h>
#include <Arduino.h>
#include "settings.h"

extern volatile uint16_t adc_vr_1;
extern volatile uint16_t adc_vr_2;
extern volatile uint16_t adc_vr_3;
extern volatile uint16_t adc_vr_4;
extern volatile uint16_t adc_vr_1_inv;
extern volatile uint16_t adc_vr_2_inv;
extern volatile uint16_t adc_vr_3_inv;
extern volatile uint16_t adc_vr_4_inv;
extern volatile bool gpio_sw_1;
extern volatile bool gpio_sw_2;
extern volatile bool gpio_push_1;
extern volatile bool gpio_push_2;

void input_init();
void input_task();
uint8_t menu_select(uint8_t mode_selector);

#endif
