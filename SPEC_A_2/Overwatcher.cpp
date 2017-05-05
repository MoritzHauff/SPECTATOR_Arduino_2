/** Overwatcher.cpp
***
*** Diese Klasse �berwacht den globalen Zustand und einzelne Aktionen. Dazu 
*** sammelt sie die Fehlermeldungen der einzelnen Komponenten und greift gegebenfalls 
*** mit Recover-Funktionen in das Geschehen ein.
***
*** Moritz Hauff, 06.04.2017
**/

// todo nach einer gewissen Zeit erneut kalibrieren.
// todo bumperkontrolle.

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
		rampenCounter = 0;

		if (SA.Motoren.GetLEDState() == LOW)
		{
			SA.Motoren.TurnLEDOn();   // Daf�r sorgen, dass nach jeder CoffeeBreak oder sonstiger M�glichkeit die Unterflufbeleuchtung wieder eingeschaltet wird.
		}
	}

	if (stateMachine->GetCurrentState() != "CoffeeBreak" && stateMachine->GetCurrentState() != "TeleOp")
	{
		// Kontrolliere ob ein schwarzes Feld befahren wird.
		//Serial.print("Aktuelle Helligkeit: ");
		//Serial.println(SA.ldr.GetValue());
		if (SA.ldr.GetValue() < C_SchwarzesFeld && stateMachine->GetCurrentState() != "SchwarzesFeld")   // todo add a counter
		{
			Serial.println("Overwatcher: Fahre vom schwarzen Feld zur�ck.");
			stateMachine->SendDirectCommand("bSFRe");  // Sende das Kommando "SchwarzesFeldRecover".
		}

		// Kontrolliere ob Rape gerade Befahren wird.  pos Pitch Wert bei Rampe hoch.
		if(SA.mpu.GetPitch() > C_Overwatcher_RampenWinkel)
		{
			rampenCounter++;
		}
		if (SA.mpu.GetPitch() < -0.15)
		{
			rampenCounter += 3;
		}
		if (rampenCounter > 20 && stateMachine->GetCurrentState() != "Rampe")
		{
			Serial.println("Overwatcher: Befahre gerade die Rampe. Aendere Status.");
			stateMachine->SendDirectCommand("bRAMe");
		}

		// todo add bumperKontroller

		if (SA.switchLinks.GetLastState() == true && stateMachine->GetCurrentState() != "ScriptedMovement")
		{
			stateMachine->SendDirectCommand("bSLe");
			Serial.println("Ausweichmanoever gestartet.");
			SA.GeradeSonstWieNichtVorangekommen = true;
		}

		// todo
	}
}

void OverwatcherClass::ErrorHandler(String Msg)
{
	if (Msg == "DrehFehler")
	{
		Serial.println("Overwatcher.ErrorHandler(): Behebe Drehfehler.");
		
		OW.stateMachine->SendDirectCommand("Dreh dich richtig zu Schmock");  // todo add the right command!
	}
}
