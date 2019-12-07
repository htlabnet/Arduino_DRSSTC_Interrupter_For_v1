#include "lib_input.h"

volatile uint16_t adc_vr_1 = DEFAULT_VR1;
volatile uint16_t adc_vr_2 = DEFAULT_VR2;
volatile uint16_t adc_vr_3 = DEFAULT_VR3;
volatile uint16_t adc_vr_4 = DEFAULT_VR4;
volatile uint16_t adc_vr_1_inv = 1013 - DEFAULT_VR1;
volatile uint16_t adc_vr_2_inv = 1013 - DEFAULT_VR2;
volatile uint16_t adc_vr_3_inv = 1013 - DEFAULT_VR3;
volatile uint16_t adc_vr_4_inv = 1013 - DEFAULT_VR4;
volatile bool gpio_sw_1 = !(DEFAULT_SW1);
volatile bool gpio_sw_2 = !(DEFAULT_SW2);
volatile bool gpio_push_1;
volatile bool gpio_push_2;

void input_init() {
  #if USE_VR1
    pinMode(PIN_VR1, INPUT);
  #endif
  #if USE_VR2
    pinMode(PIN_VR2, INPUT);
  #endif
  #if USE_VR3
    pinMode(PIN_VR3, INPUT);
  #endif
  #if USE_VR4
    pinMode(PIN_VR4, INPUT);
  #endif
  #if USE_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
  #endif
  #if USE_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
  #endif
  #if USE_PUSH1
    pinMode(PIN_PUSH1, INPUT_PULLUP);
  #endif
  #if USE_PUSH2
    pinMode(PIN_PUSH2, INPUT_PULLUP);
  #endif
}


void input_task() {
  // Read Volume
  #if USE_VR1
    #if !INVERT_VR1
      adc_vr_1 = ((float)adc_vr_1 * 0.9) + ((float)analogRead(PIN_VR1) * 0.1);
      adc_vr_1_inv = ((float)adc_vr_1_inv * 0.9) + ((float)(1023 - analogRead(PIN_VR1)) * 0.1);
    #else
      adc_vr_1 = ((float)adc_vr_1 * 0.9) + ((float)(1023 - analogRead(PIN_VR1)) * 0.1);
      adc_vr_1_inv = ((float)adc_vr_1_inv * 0.9) + ((float)analogRead(PIN_VR1) * 0.1);
    #endif
  #endif
  #if USE_VR2
    #if !INVERT_VR2
      adc_vr_2 = ((float)adc_vr_2 * 0.9) + ((float)analogRead(PIN_VR2) * 0.1);
      adc_vr_2_inv = ((float)adc_vr_2_inv * 0.9) + ((float)(1023 - analogRead(PIN_VR2)) * 0.1);
    #else
      adc_vr_2 = ((float)adc_vr_2 * 0.9) + ((float)(1023 - analogRead(PIN_VR2)) * 0.1);
      adc_vr_2_inv = ((float)adc_vr_2_inv * 0.9) + ((float)analogRead(PIN_VR2) * 0.1);
    #endif
  #endif
  #if USE_VR3
    #if !INVERT_VR3
      adc_vr_3 = ((float)adc_vr_3 * 0.9) + ((float)analogRead(PIN_VR3) * 0.1);
      adc_vr_3_inv = ((float)adc_vr_3_inv * 0.9) + ((float)(1023 - analogRead(PIN_VR3)) * 0.1);
    #else
      adc_vr_3 = ((float)adc_vr_3 * 0.9) + ((float)(1023 - analogRead(PIN_VR3)) * 0.1);
      adc_vr_3_inv = ((float)adc_vr_3_inv * 0.9) + ((float)analogRead(PIN_VR3) * 0.1);
    #endif
  #endif
  #if USE_VR4
    #if !INVERT_VR4
      adc_vr_4 = ((float)adc_vr_4 * 0.9) + ((float)analogRead(PIN_VR4) * 0.1);
      adc_vr_4_inv = ((float)adc_vr_4_inv * 0.9) + ((float)(1023 - analogRead(PIN_VR4)) * 0.1);
    #else
      adc_vr_4 = ((float)adc_vr_4 * 0.9) + ((float)(1023 - analogRead(PIN_VR4)) * 0.1);
      adc_vr_4_inv = ((float)adc_vr_4_inv * 0.9) + ((float)analogRead(PIN_VR4) * 0.1);
    #endif
  #endif
  #if USE_SW1
    #if !INVERT_SW1
      gpio_sw_1 = (bool)digitalRead(PIN_SW1);
    #else
      gpio_sw_1 = !(bool)digitalRead(PIN_SW1);
    #endif
  #endif
  #if USE_SW2
    #if !INVERT_SW2
      gpio_sw_2 = (bool)digitalRead(PIN_SW2);
    #else
      gpio_sw_2 = !(bool)digitalRead(PIN_SW2);
    #endif
  #endif
  #if USE_PUSH1
    #if !INVERT_PUSH1
      gpio_push_1 = (bool)digitalRead(PIN_PUSH1);
    #else
      gpio_push_1 = !(bool)digitalRead(PIN_PUSH1);
    #endif
  #endif
  #if USE_PUSH2
    #if !INVERT_PUSH2
      gpio_push_2 = (bool)digitalRead(PIN_PUSH2);
    #else
      gpio_push_2 = !(bool)digitalRead(PIN_PUSH2);
    #endif
  #endif
}


uint8_t menu_select(uint8_t mode_selector) {

  // OSC Mode
  if (gpio_sw_1 && gpio_sw_2) {
    // Volume Mode Select
    switch (adc_vr_4 >> (mode_selector + 8)) {
      case 0:
        return MODE_OSC;
      case 1:
        return MODE_OSC_OS;
      case 2:
        return MODE_OSC_HP;
      case 3:
        return MODE_OSC_HP_OS;
    }
  }
  // Burst OSC Mode
  if (gpio_sw_1 && !gpio_sw_2) {
    return MODE_BURST;
  }
  // MIDI Mode
  if (!gpio_sw_1 && gpio_sw_2) {
    return MODE_MIDI;
  }
  // MIDI Fixed Mode
  if (!gpio_sw_1 && !gpio_sw_2) {
    return MODE_MIDI_FIXED;
  }
}
