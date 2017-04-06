// Overwatcher.h - Moritz Hauff - 06.04.2017

#ifndef _OVERWATCHER_h
#define _OVERWATCHER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SPECTATORClass.h"
#include "StateMachine.h"

///////////////////////////////////////////////////////////////////////////
///Overwatcher-Class
/*Diese Klasse überwacht den globalen Zustand und einzelne Aktionen. Dazu 
sammelt sie die Fehlermeldungen der einzelnen Komponenten und greift gegebenfalls 
mit Recover-Funktionen in das Geschehen ein.*/
class OverwatcherClass
{
 protected:
	 StateMachineClass *stateMachine;

	 int actions;   // beeinhalt die seit dem ArduinoStart durchgeführte Aktionen.

 public:
	 void Init(StateMachineClass *StateMachine);

	 /*Führt eine Statuskontrolle durch. Dies sollte in jedem (2.) loop Durchlauf geschehen.*/
	 void Control();

	 int GetActions() { return actions; }
};

extern OverwatcherClass OW;

#endif
