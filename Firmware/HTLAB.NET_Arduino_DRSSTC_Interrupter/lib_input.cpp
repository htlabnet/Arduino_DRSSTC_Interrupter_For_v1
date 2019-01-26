#include "lib_input.h"

volatile unsigned int adc_vr_1 = DEFAULT_VR1;
volatile unsigned int adc_vr_2 = DEFAULT_VR2;
volatile unsigned int adc_vr_3 = DEFAULT_VR3;
volatile unsigned int adc_vr_4 = DEFAULT_VR4;
volatile unsigned int adc_vr_1_inv = 1023 - DEFAULT_VR1;
volatile unsigned int adc_vr_2_inv = 1023 - DEFAULT_VR2;
volatile unsigned int adc_vr_3_inv = 1023 - DEFAULT_VR3;
volatile unsigned int adc_vr_4_inv = 1023 - DEFAULT_VR4;
volatile boolean gpio_sw_1 = DEFAULT_SW1;
volatile boolean gpio_sw_2 = DEFAULT_SW2;
volatile boolean gpio_push_1;
volatile boolean gpio_push_2;

void input_init() {
  #if USE_VR1
    pinMode(A0, INPUT);
  #endif
  #if USE_VR2
    pinMode(A1, INPUT);
  #endif
  #if USE_VR3
    pinMode(A2, INPUT);
  #endif
  #if USE_VR4
    pinMode(A3, INPUT);
  #endif
  #if USE_SW1
    pinMode(A4, INPUT_PULLUP);
  #endif
  #if USE_SW2
    pinMode(A5, INPUT_PULLUP);
  #endif
  #if USE_PUSH1
    pinMode(2, INPUT_PULLUP);
  #endif
  #if USE_PUSH2
    pinMode(3, INPUT_PULLUP);
  #endif
}


void input_task() {
  // Read Volume
  #if USE_VR1
    #if !INVERT_VR1
      adc_vr_1 = ((float)adc_vr_1 * 0.9) + ((float)analogRead(A0) * 0.1);
      adc_vr_1_inv = ((float)adc_vr_1_inv * 0.9) + ((float)(1023 - analogRead(A0)) * 0.1);
    #else
      adc_vr_1 = ((float)adc_vr_1 * 0.9) + ((float)(1023 - analogRead(A0)) * 0.1);
      adc_vr_1_inv = ((float)adc_vr_1_inv * 0.9) + ((float)analogRead(A0) * 0.1);
    #endif
  #endif
  #if USE_VR2
    #if !INVERT_VR2
      adc_vr_2 = ((float)adc_vr_2 * 0.9) + ((float)analogRead(A1) * 0.1);
      adc_vr_2_inv = ((float)adc_vr_2_inv * 0.9) + ((float)(1023 - analogRead(A1)) * 0.1);
    #else
      adc_vr_2 = ((float)adc_vr_2 * 0.9) + ((float)(1023 - analogRead(A1)) * 0.1);
      adc_vr_2_inv = ((float)adc_vr_2_inv * 0.9) + ((float)analogRead(A1) * 0.1);
    #endif
  #endif
  #if USE_VR3
    #if !INVERT_VR3
      adc_vr_3 = ((float)adc_vr_3 * 0.9) + ((float)analogRead(A2) * 0.1);
      adc_vr_3_inv = ((float)adc_vr_3_inv * 0.9) + ((float)(1023 - analogRead(A2)) * 0.1);
    #else
      adc_vr_3 = ((float)adc_vr_3 * 0.9) + ((float)(1023 - analogRead(A2)) * 0.1);
      adc_vr_3_inv = ((float)adc_vr_3_inv * 0.9) + ((float)analogRead(A2) * 0.1);
    #endif
  #endif
  #if USE_VR4
    #if !INVERT_VR4
      adc_vr_4 = ((float)adc_vr_4 * 0.9) + ((float)analogRead(A3) * 0.1);
      adc_vr_4_inv = ((float)adc_vr_4_inv * 0.9) + ((float)(1023 - analogRead(A3)) * 0.1);
    #else
      adc_vr_4 = ((float)adc_vr_4 * 0.9) + ((float)(1023 - analogRead(A3)) * 0.1);
      adc_vr_4_inv = ((float)adc_vr_4_inv * 0.9) + ((float)analogRead(A3) * 0.1);
    #endif
  #endif
  #if USE_SW1
    #if !INVERT_SW1
      gpio_sw_1 = digitalRead(A4);
    #else
      gpio_sw_1 = !digitalRead(A4);
    #endif
  #endif
  #if USE_SW2
    #if !INVERT_SW2
      gpio_sw_2 = digitalRead(A5);
    #else
      gpio_sw_2 = !digitalRead(A5);
    #endif
  #endif
  #if USE_PUSH1
    #if !INVERT_PUSH1
      gpio_push_1 = digitalRead(2);
    #else
      gpio_push_1 = !digitalRead(2);
    #endif
  #endif
  #if USE_PUSH2
    #if !INVERT_PUSH2
      gpio_push_2 = digitalRead(3);
    #else
      gpio_push_2 = !digitalRead(3);
    #endif
  #endif
}


byte menu_select() {

  // OSC Mode
  if (gpio_sw_1 && gpio_sw_2) {
    // Volume Mode Select
    switch (adc_vr_4 >> 8) {
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
  // MIDI Mode
  if (!gpio_sw_1 && !gpio_sw_2) {
    return MODE_MIDI;
  }
}
