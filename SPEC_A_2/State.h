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

/*Beeinhaltet den momentanen Ausführungszustand eines States an.*/
enum StateStatus {
	Running, Finished, Aborted, Error
};

///////////////////////////////////////////////////////////////////////////
///State-Class
class StateClass  // abstrakte Klasse mit virtuellen Funktionen
{
private:
	 String name;
 protected:
	 SPECTATORClass *spectator;

	 StateStatus status;

 public:
	 StateClass(SPECTATORClass *Spectator, const char Name[]);
	 // todo: virtueller Destruktor?

	 virtual void Sense() = 0;  // noch keine Methodenimplemtierung -> siehe vererbte Klassen
	 virtual void Think() = 0;
	 virtual void Act() = 0;
	 /*Wird immer aufgerufen wenn in den Modus gewechselt wird.*/
	 virtual void Init() = 0;
	 /*Wird nach der Wiederaufnahme des Modus nach der KaffeePause aufgerufen 
	 und verschiebt die Timer um die angegebene Zeit [ms].*/
	 virtual void ShiftTimers(unsigned long ShiftAmount) = 0;

	 String GetName();
	 StateStatus GetStatus();
};

#endif
