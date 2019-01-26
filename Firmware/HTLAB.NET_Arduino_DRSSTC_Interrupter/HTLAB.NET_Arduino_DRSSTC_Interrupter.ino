//
// #################################################
//
//    HTLAB.NET Arduino DRSSTC Interrupter
//      http://htlab.net/electronics/
//
//    Copyright (C) 2017
//      Hideto Kikuchi / PJ (@pcjpnet) - http://pc-jp.net/
//      Tsukuba Science Inc. - http://www.tsukuba-kagaku.co.jp/
//
//    !!!!! NOT ALLOWED COMMERCIAL USE !!!!!
//
// #################################################
//

//
// ########## Compatible Boards ##########
//    - Arduino Leonardo (USB-MIDI is available)
//    - Arduino Micro (USB-MIDI is available)
//

//
// ########## Pin Assignments ##########
//  D2  - PUSH1
//  D3  - PUSH2
//  D4  - LCD(RS)
//  D5  - LCD(ENA)
//  D6  - LCD(DB4)
//  D7  - LCD(DB5)
//  D8  - LCD(DB6)
//  D9  - LCD(DB7)
//  D10 - OUT1 (MIDI CH:1)
//  D11 - OUT2 (MIDI CH:2)
//  D12 - (Reserve:OUT3)
//  D13 - (Reserve:OUT4)
//  A0  - VR1
//  A1  - VR2
//  A2  - VR3
//  A3  - VR4
//  A4  - SW1
//  A5  - SW2
//

//
// ########## Require Libraries ##########
//    - MIDI Library 4.3
//
//

//
// ########## Optional Libraries ##########
//    - MIDIUSB Library 1.0.3 (for Arduino Leonardo, Micro)
//
//

// LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

// Settings
#include "settings.h"

// Pin I/O
#include "lib_input.h"
#include "lib_output.h"

// Oscillator
#include "lib_osc.h"

// MIDI Parameter
#include "lib_midi.h"

// Use MIDI Library
#include <MIDI.h>
#if (MIDI_LIBRARY_VERSION_MAJOR < 4) || \
  ((MIDI_LIBRARY_VERSION_MAJOR == 4) && (MIDI_LIBRARY_VERSION_MINOR < 3))
  #error This version of MIDI library is not supported. Please use version 4.3 or later.
#endif

// Use MIDIUSB Library (for Arduino Leonardo, Micro)
#if USE_MIDIUSB && defined(USBCON) && defined(__AVR_ATmega32U4__)
  #include <midi_UsbTransport.h>
  static const unsigned sUsbTransportBufferSize = 16;
  typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;
  UsbTransport sUsbTransport;
  MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDI);
#else
  MIDI_CREATE_DEFAULT_INSTANCE();
#endif


// Global Variables
volatile byte int_mode = 0;
volatile byte menu_read = 0;
char lcd_line1[17];
char lcd_line2[17];

// OSC Mode Variables
volatile unsigned int osc_fq = 5;
volatile unsigned int osc_us = 1;
volatile unsigned int osc_per = 25000;
volatile unsigned int osc_per_read = 25000;

// MIDI Mode Variables
volatile boolean use_midi_volume = USE_MIDI_VOLUME;
//volatile boolean osc_mode_omni = OSC_MODE_OMNI;
volatile boolean osc_mode_fixed = OSC_MODE_FIXED;
volatile byte osc_mono_midi_note[2] = {0, 0};
volatile byte osc_mono_midi_volume[2] = {64, 64};
volatile byte osc_mono_midi_expression[2] = {127, 127};
volatile unsigned int osc_mono_ontime_us[2] = {0, 0};
volatile unsigned int osc_mono_ontime_max_us[2] = {OSC_ONTIME_US_1, OSC_ONTIME_US_2};
volatile unsigned long osc_mono_fixed_ontime_max_us[2] = {OSC_FIXED_ONTIME_US_1, OSC_FIXED_ONTIME_US_2};


// Arduino Setup Function
void setup() {
  
  // Pin Init
  input_init();
  output_init();

  // LCD
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("HTLAB.NET DRSSTC");
  lcd.setCursor(0,1);
  lcd.print("Interrupter v1.0");
  delay(1000);

  // For Debug
  #if DEBUG_SERIAL
    Serial.begin(115200);
    #if DEBUG_SERIAL_WAIT
      while (!Serial);
      Serial.println("[INFO] Arduino Start");
    #endif
  #endif
  
  // Use MIDI Library
  MIDI.setHandleNoteOn(isr_midi_noteon);
  MIDI.setHandleNoteOff(isr_midi_noteoff);
  MIDI.setHandleControlChange(isr_midi_controlchange);
  MIDI.setHandleActiveSensing(isr_midi_activesensing);
  MIDI.setHandleSystemReset(isr_midi_systemreset);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  #if DEBUG_SERIAL
    Serial.println("[INFO] MIDI Library Load Complete");
  #endif
  
  // Oscillator Tasks
  osc_timer_init();
  #if DEBUG_SERIAL
    Serial.println("[INFO] Oscillator Tasks Complete");
  #endif

  mode_init(menu_read);
  
}


// Arduino Main Loop
void loop() {

  // Input Tasks
  input_task();
  menu_read = menu_select();
  if (int_mode != menu_read) {
    mode_init(menu_read);
    int_mode = menu_read;
  }

  // OSC Tasks
  switch (int_mode) {
    case 0:
      osc_fq = adc_vr_1 + 5;
      osc_us = (adc_vr_2 >> 2) + 1;
      osc_per_read = 250000 / osc_fq;
      if (osc_per != osc_per_read) {
        osc_per = osc_per_read;
        OCR1A = osc_per;
        OCR3A = osc_per;
      }
      break;
    
  }



  
  //Serial.println(adc_vr_4 >> 8); //0-3
  //Serial.println(gpio_sw_1);
  
  // MIDI Tasks
  MIDI.read();
  
  // LCD
  show_lcd(int_mode);
  
}


// MODE Init
void mode_init(byte mode) {
  switch (mode) {
    // OSC Mode
    case 0:
      osc_timer_disable(0);
      osc_timer_disable(1);
      osc_timer_init_64();
      osc_timer_enable(0, osc_per);
      osc_timer_enable(1, osc_per);
      break;
    // MIDI Mode
    case 32:
      osc_timer_disable(0);
      osc_timer_disable(1);
      osc_timer_init();
  }
  #if DEBUG_SERIAL
    Serial.print("[INFO] Change Mode : ");
    Serial.println(mode);
  #endif
}


// SHOW LCD
void show_lcd(byte mode) {
  switch (mode) {
    // OSC Mode
    case 0:
      sprintf(lcd_line1, "OSC:%4uHz/%3uus", osc_fq, osc_us);
      sprintf(lcd_line2, "OSC MODE        ");
      break;

    // Burst OSC Mode
    case 16:
      sprintf(lcd_line1, "OSC:%4uHz/%3uus", osc_fq, osc_us);
      sprintf(lcd_line2, "BURST%3ums/%3ums", adc_vr_3/2, adc_vr_4/2);
      break;

    // MIDI Mode
    case 32:
      sprintf(lcd_line1, "MID:%4uHz/%3uus", adc_vr_1+5, adc_vr_2/4);
      sprintf(lcd_line2, "MIDI MODE       ");
      break;
  }
  lcd.setCursor(0,0);
  lcd.print(lcd_line1);
  lcd.setCursor(0,1);
  lcd.print(lcd_line2);
}


// Interrupt Tasks
ISR (TIMER1_COMPA_vect) {
  switch (int_mode) {
    case 0:
      output_single_pulse(0, osc_us);
      break;
    case 32:
      output_single_pulse(0, osc_mono_ontime_us[0]);
      break;
  }
}


// Interrupt Tasks
ISR (TIMER3_COMPA_vect) {
  switch (int_mode) {
    case 0:
      output_single_pulse(1, osc_us);
      break;
    case 32:
      output_single_pulse(1, osc_mono_ontime_us[1]);
      break;
  }
}


// Interrupt MIDI NoteON Tasks
void isr_midi_noteon(byte ch, byte num, byte vel) {

  if (ch == 1 || ch == 2) {
    if (osc_mono_midi_note[ch - 1] == num) {
      osc_timer_disable(ch - 1);
    }
    if (vel > 0){
      if (osc_mode_fixed) {
        if (use_midi_volume) {
          float volume = ((float)vel * (float)osc_mono_midi_volume[ch - 1] * (float)osc_mono_midi_expression[ch - 1]) / (float)2048383;
          osc_mono_ontime_us[ch - 1] = (osc_mono_fixed_ontime_max_us[ch - 1] / (unsigned long)note_to_hz[num]) * volume;
        } else {
          osc_mono_ontime_us[ch - 1] = osc_mono_fixed_ontime_max_us[ch - 1] / (unsigned long)note_to_hz[num];
        }
      } else {
        if (use_midi_volume) {
          float volume = ((float)vel * (float)osc_mono_midi_volume[ch - 1] * (float)osc_mono_midi_expression[ch - 1]) / (float)2048383;
          osc_mono_ontime_us[ch - 1] = osc_mono_ontime_max_us[ch - 1] * volume;
        } else {
          osc_mono_ontime_us[ch - 1] = osc_mono_ontime_max_us[ch - 1];
        }
      }
      osc_timer_enable(ch - 1, timer_period[num] - 1);
      osc_mono_midi_note[ch - 1] = num;
      
    }
  }
  
  // For Debug
  #if DEBUG_SERIAL
    Serial.print("[MIDI] Note On - CH:");
    Serial.print(ch);
    Serial.print(" NUM:");
    Serial.print(num);
    Serial.print(" VEL:");
    Serial.print(vel);
    Serial.print(" / ");
    Serial.print(note_to_hz[num]);
    Serial.println(" Hz");
  #endif
}


// Interrupt MIDI NoteOFF Tasks
void isr_midi_noteoff(byte ch, byte num, byte vel) {

  if (ch == 1 || ch == 2) {
    if (osc_mono_midi_note[ch - 1] == num) {
      osc_timer_disable(ch - 1);
    }
  }

  // For Debug
  #if DEBUG_SERIAL
    Serial.print("[MIDI] NoteOff - CH:");
    Serial.print(ch);
    Serial.print(" NUM:");
    Serial.print(num);
    Serial.print(" VEL:");
    Serial.println(vel);
  #endif
}



void isr_midi_controlchange(byte ch, byte num, byte val) {
  
  switch(num) {
    case 7:   // CC#7   Channel Volume
      osc_mono_midi_volume[ch - 1] = val;
      break;
    case 11:  // CC#11  Expression
      osc_mono_midi_expression[ch - 1] = val;
      break;
    case 120: // CC#120 All Sound Off
    case 123: // CC#123 All Notes Off
      osc_timer_disable(0);
      osc_timer_disable(1);
      break;
    case 121: // CC#121 Reset All Controllers
      osc_mono_midi_volume[0] = 64;
      osc_mono_midi_volume[1] = 64;
      osc_mono_midi_expression[0] = 127;
      osc_mono_midi_expression[0] = 127;
      break;
    case 124: // CC#124 Omni Mode Off
      //osc_mode_omni = false;
      break;
    case 125: // CC#125 Omni Mode On
      //osc_mode_omni = true;
      break;
  }
  
  // For Debug
  #if DEBUG_SERIAL
    Serial.print("[MIDI] CC - CH:");
    Serial.print(ch);
    Serial.print(" NUM:");
    Serial.print(num);
    Serial.print(" VAL:");
    Serial.println(val);
  #endif
}


void isr_midi_activesensing() {


  // For Debug
  #if DEBUG_SERIAL
    Serial.println("[MIDI] Active Sensing");
  #endif
}


void isr_midi_systemreset() {
  
  osc_timer_disable(0);
  osc_timer_disable(1);
  //osc_mode_omni = OSC_MODE_OMNI;
  osc_mono_midi_volume[0] = 64;
  osc_mono_midi_volume[1] = 64;
  osc_mono_midi_expression[0] = 127;
  osc_mono_midi_expression[0] = 127;
  
  // For Debug
  #if DEBUG_SERIAL
    Serial.println("[MIDI] System Reset");
  #endif
}
