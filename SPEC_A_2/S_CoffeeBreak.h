// S_CoffeeBreak.h - Moritz Hauff - 06.04.2017

#ifndef _S_COFFEEBREAK_h
#define _S_COFFEEBREAK_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "State.h"

///////////////////////////////////////////////////////////////////////////
///Konstanten
const int BlinkDelay = 400;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Diese Klasse ermöglicht dem seriellen Operator die aktuelle Aktion zu 
unterbrechen und sie nach einem weiteren Befehl wieder aufzunehmen.
Dies soll dem Debugging dienen.*/
class S_CoffeeBreakClass : public StateClass
{
 protected:
	 unsigned long coffeBreakStart;
	 unsigned long lastToggle;

 public:
	 S_CoffeeBreakClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }
	 
	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount) { /* in the CoffeeBreak are no timers. */ }

	 /*Gibt die verstrichenen Zeit [ms] seit der Aktivierung der KaffePause zurück,
	 um beim Wiederaufnehmen der alten Aktion die Timer anpassen zu können.*/
	 unsigned long GetTimerShiftAmount();

	 StateClass *LastState;
};

#endif
