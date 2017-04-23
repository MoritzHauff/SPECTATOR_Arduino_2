/** Overwatcher.cpp
***
*** Diese Klasse �berwacht den globalen Zustand und einzelne Aktionen. Dazu 
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
	stateMachine->OverwatcherMsg = &this->ErrorHandler;

	stateMachine->SendDirectCommand("bCALe");  // Testbefehl (Kalibrierung) an StateMachine senden.

	SA.Motoren.TurnLEDOn();
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void OverwatcherClass::Control()
{
	if (stateMachine->StartedNewAction())
	{
		actions++;
	}

	// Kontrolliere ob ein schwarzes Feld befahren wird.
	//Serial.print("Aktuelle Helligkeit: ");
	//Serial.println(SA.ldr.GetValue());
	if (SA.ldr.GetValue() < C_SchwarzesFeld && stateMachine->GetCurrentState() != "SchwarzesFeld")   // todo add a counter
	{
		stateMachine->SendDirectCommand("bSFRe");  // Sende das Kommando "SchwarzesFeldRecover".
		Serial.println("Fahre vom schwarzen Feld zur�ck.");
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
