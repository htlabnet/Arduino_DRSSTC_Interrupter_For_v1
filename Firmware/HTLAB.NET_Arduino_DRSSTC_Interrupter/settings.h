#ifndef SETTINGS_H
#define SETTINGS_H

// ########## Interrupter Settings ##########

// Use Volume Input (DEFAULT = 0-1023)
#define USE_VR1 true
#define USE_VR2 true
#define USE_VR3 true
#define USE_VR4 true
#define INVERT_VR1 false
#define INVERT_VR2 false
#define INVERT_VR3 false
#define INVERT_VR4 false
#define DEFAULT_VR1 0
#define DEFAULT_VR2 0
#define DEFAULT_VR3 0
#define DEFAULT_VR4 0

#define USE_SW1 true
#define USE_SW2 true
#define INVERT_SW1 false
#define INVERT_SW2 false
#define DEFAULT_SW1 false
#define DEFAULT_SW2 false

#define USE_PUSH1 true
#define USE_PUSH2 true
#define INVERT_PUSH1 false
#define INVERT_PUSH2 false

// Use Setting Mode
#define USE_SETTING_MODE true

// Interrupter Mode Selector
//  8 : 4-Mode [OSC, OSC_OneShot, HighPower_OSC, HighPower_OSC_OneShot]
//  9 : 2-Mode [OSC, OSC_OneShot]
// 10 : 1-Mode [OSC]
#define INTERRUPTER_MODE_SELECTOR 8

// Use MIDIUSB Library
#define USE_MIDIUSB true

// Use MIDI Volume (CC#7 & CC#11 & Velocity)
#define USE_MIDI_VOLUME false

// Fixed Mode
#define OSC_MODE_FIXED false

// Max ON-time (us)
#define OSC_ONTIME_US_1 10
#define OSC_ONTIME_US_2 10

// Max ON-time Per Sec for Fixed Mode (us)
#define OSC_FIXED_ONTIME_US_1 10000
#define OSC_FIXED_ONTIME_US_2 10000


// ########## Settings Complete! ##########

// 8 or 64
#define OSC_TIMER_DIVIDER 8

// For Debug
#define DEBUG_SERIAL false

// Wait for Serial Port
#define DEBUG_SERIAL_WAIT false

// Mode (Do not change the value)
#define MODE_OSC 1
#define MODE_OSC_OS 2
#define MODE_OSC_HP 4
#define MODE_OSC_HP_OS 8
#define MODE_BURST 16
#define MODE_MIDI 32

// Pin Number (Do not change the value)
#define PIN_VR1 A0
#define PIN_VR2 A1
#define PIN_VR3 A2
#define PIN_VR4 A3
#define PIN_SW1 A4
#define PIN_SW2 A5
#define PIN_PUSH1 2 //Interrupt1
#define PIN_PUSH2 3 //Interrupt0

#endif
