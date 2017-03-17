// State.h - Moritz Hauff - 17.03.2017

#ifndef _STATE_h
#define _STATE_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SPECTATORClass.h"

///////////////////////////////////////////////////////////////////////////
///State-Class
class StateClass  // abstrakte Klasse mit virtuellen Funktionen
{
 protected:
	 SPECTATORClass *spectator;

 public:
	 StateClass(SPECTATORClass *Spectator);
	 // todo: virtueller Destruktor?

	 virtual void Sense() = 0;  // noch keine Methodenimplemtierung -> siehe vererbte Klassen
	 virtual void Think() = 0;
	 virtual void Act() = 0;
	 /*Wird immer aufgerufen wenn in den Modus gewechselt wird.*/
	 virtual void Init() = 0;
};

#endif
