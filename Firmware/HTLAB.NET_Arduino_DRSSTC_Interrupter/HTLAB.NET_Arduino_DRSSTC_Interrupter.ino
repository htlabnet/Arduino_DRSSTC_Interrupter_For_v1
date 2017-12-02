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
//    - Arduino Uno Rev3 (USB-MIDI is available using MOCO)
//    - Arduino Leonardo (USB-MIDI is available)
//    - Arduino Micro (USB-MIDI is available)
//

//
// ########## Pin Assignments ##########
//  D8  - OUT1 (MIDI CH:1)
//  D9  - OUT2 (MIDI CH:2)
//  D10 - (Reserve:OUT3)
//  D11 - (Reserve:OUT4)
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


// Settings
#include "settings.h"

// Pin I/O
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
#if defined(USE_MIDIUSB) && defined(USBCON) && \
    (defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO))
  #include <midi_UsbTransport.h>
  static const unsigned sUsbTransportBufferSize = 16;
  typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;
  UsbTransport sUsbTransport;
  MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDI);
#else
  MIDI_CREATE_DEFAULT_INSTANCE();
#endif


// Global Variables
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
  output_init();
  
  // For Debug
  #ifdef DEBUG_SERIAL
    Serial.begin(115200);
    #ifdef DEBUG_SERIAL_WAIT
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
  #ifdef DEBUG_SERIAL
    Serial.println("[INFO] MIDI Library Load Complete");
  #endif
  
  // Oscillator Tasks
  osc_timer_init();
  #ifdef DEBUG_SERIAL
    Serial.println("[INFO] Oscillator Tasks Complete");
  #endif
  
}


// Arduino Main Loop
void loop() {
  
  // MIDI Tasks
  MIDI.read();
  
  
}


// Interrupt Tasks
ISR (TIMER1_COMPA_vect) {
  output_single_pulse(0, osc_mono_ontime_us[0]);
}


// Interrupt Tasks
ISR (TIMER3_COMPA_vect) {
  output_single_pulse(1, osc_mono_ontime_us[1]);
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
  #ifdef DEBUG_SERIAL
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
  #ifdef DEBUG_SERIAL
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
  #ifdef DEBUG_SERIAL
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
  #ifdef DEBUG_SERIAL
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
  #ifdef DEBUG_SERIAL
    Serial.println("[MIDI] System Reset");
  #endif
}




