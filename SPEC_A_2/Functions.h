// Functions.h - Moritz Hauff - 17.02.2017
// see Functions.c

#ifndef _FUNCTIONS_h
#define _FUNCTIONS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

void SendToRP(uint8_t Code, int Value);

#endif

