// Functions.h - Moritz Hauff - 17.02.2017
// see Functions.c

#ifndef _FUNCTIONS_h
#define _FUNCTIONS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#include <avr/io.h>

//Initialisiert den Analog-Digital-Wandler mit einer genaueren Einstallbarkeit als bei analogRead().
extern void init_adc(void);

//Etwas besser einstellbares analogRead().
//Bevor diese Funktion verwendet werden kann muss init_adc() aufgerufen werden.
extern int16_t get_adc(uint8_t channel);
// für weitere Informationen siehe: https://bennthomsen.wordpress.com/arduino/peripherals/analogue-input/

#endif

