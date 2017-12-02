#ifndef SETTINGS_H
#define SETTINGS_H

// ########## Interrupter Settings ##########

// Use MIDIUSB Library
#define USE_MIDIUSB

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

// For Debug (Arduino Leonardo, Micro)
//#define DEBUG_SERIAL

// Wait for Serial Port
//#define DEBUG_SERIAL_WAIT

#endif


