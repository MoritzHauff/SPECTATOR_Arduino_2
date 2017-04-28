/** Overwatcher.cpp
***
*** Diese Klasse �berwacht den globalen Zustand und einzelne Aktionen. Dazu 
*** sammelt sie die Fehlermeldungen der einzelnen Komponenten und greift gegebenfalls 
*** mit Recover-Funktionen in das Geschehen ein.
***
*** Moritz Hauff, 06.04.2017
**/

///////////////////////////////////////////////////////////////////////////
/// Copyright (C) {2017}  {Moritz Hauff}
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// If you start using parts of this code please write a short message to: 
/// license@vierradroboter.de
///
/// If you have any questions contact me via mail: admin@vierradroboter.de
///////////////////////////////////////////////////////////////////////////

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

		// Kontrolliere ob Rape gerade Befahren wird.
		if (SA.mpu.GetPitch() < -C_Overwatcher_RampenWinkel || SA.mpu.GetPitch() > C_Overwatcher_RampenWinkel)
		{
			rampenCounter++;
		}
		if (rampenCounter > 10 && stateMachine->GetCurrentState() != "Rampe")
		{
			Serial.println("Overwatcher: Befahre gerade die Rampe. Aendere Status.");
			stateMachine->SendDirectCommand("bRAMe");
		}

		// todo add bumperKontroller
		//stateMachine->SendDirectCommand("bSLe");

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
