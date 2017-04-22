/** Overwatcher.cpp
***
*** Diese Klasse überwacht den globalen Zustand und einzelne Aktionen. Dazu 
*** sammelt sie die Fehlermeldungen der einzelnen Komponenten und greift gegebenfalls 
*** mit Recover-Funktionen in das Geschehen ein.
***
*** Moritz Hauff, 06.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "Overwatcher.h"

///////////////////////////////////////////////////////////////////////////
///Instanz
OverwatcherClass OW; // Die HauptInstanz des Overwachters

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void OverwatcherClass::Init(StateMachineClass *StateMachine)
{
	Serial.println("Initialisiere Overwatcher ...");
	
	actions = 0;
	
	stateMachine = StateMachine;

	stateMachine->SendDirectCommand("bCALe");  // Testbefehl (Kalibrierung) an StateMachine senden.

	stateMachine->OverwatcherMsg = &this->ErrorHandler;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void OverwatcherClass::Control()
{
	if (stateMachine->StartedNewAction())
	{
		actions++;
	}

	// todo
}

void OverwatcherClass::ErrorHandler(String Msg)
{
	if (Msg == "DrehFehler")
	{
		Serial.println("Behebe Drehfehler");
		// todo do something ...
	}
}
