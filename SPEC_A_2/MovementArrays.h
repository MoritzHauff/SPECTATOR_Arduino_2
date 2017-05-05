// MovementArrays.h - Moritz Hauff - 27.04.2017

#ifndef _MOVEMENTARRAYS_h
#define _MOVEMENTARRAYS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

const int bumperLinksRows = 29;  // you need to adapt this!
const int L_bumperLinks[] = { -130, -130, -130, -130, -130, -130, -130, -130, -130, -130, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -100, -100, -100, -100, -100 };
const int R_bumperLinks[] = { -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120, -120 };


#endif
