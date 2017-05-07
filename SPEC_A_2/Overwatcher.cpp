/** Overwatcher.cpp
***
*** Diese Klasse überwacht den globalen Zustand und einzelne Aktionen. Dazu 
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

	drehFehlerCounter = 0;
	drehFehlerResetCounter = 0;
	rampenCounter = 0;

	
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

		if (drehFehlerCounter > 0)
		{
			drehFehlerResetCounter++;
		}
		if (drehFehlerResetCounter >= 10)
		{
			drehFehlerCounter = 0;  // beide Werte zurücksetzen.
			drehFehlerResetCounter = 0;
		}

		if (SA.Motoren.GetLEDState() == LOW)
		{
			SA.Motoren.TurnLEDOn();   // Dafür sorgen, dass nach jeder CoffeeBreak oder sonstiger Möglichkeit die Unterflufbeleuchtung wieder eingeschaltet wird.
		}
	}

	if (stateMachine->GetCurrentState() != "CoffeeBreak" && stateMachine->GetCurrentState() != "TeleOp")
	{
		// Kontrolliere ob ein schwarzes Feld befahren wird.
		//Serial.print("Aktuelle Helligkeit: ");
		//Serial.println(SA.ldr.GetValue());
		if (SA.ldr.GetValue() < C_SchwarzesFeld && stateMachine->GetCurrentState() != "SchwarzesFeld")   // todo add a counter
		{
			Serial.println("Overwatcher: Fahre vom schwarzen Feld zurück.");
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
			Serial.println(F("Overwatcher: Befahre gerade die Rampe. Aendere Status."));
			if (SA.mpu.GetPitch() < 0)  
			{
				stateMachine->SendDirectCommand("bRADe");  // beim runterfahrnee erst schneller fahren und dann abbremsen.
			}
			else
			{
				stateMachine->SendDirectCommand("bRAUe");
			}
		}

		// todo add bumperKontroller

		if (SA.switchLinks.GetLastState() == true && stateMachine->GetCurrentState() == "GeradeAus")
		{
			stateMachine->SendDirectCommand("bSLe");
			Serial.println(F("Ausweichmanoever für linken Trigger gestartet."));
			SA.GeradeSonstWieNichtVorangekommen = true;
		}
		if (SA.switchRechts.GetLastState() == true && stateMachine->GetCurrentState() == "GeradeAus")
		{
			stateMachine->SendDirectCommand("bSRe");
			Serial.println(F("Ausweichmanoever für rechten Trigger gestartet."));
			SA.GeradeSonstWieNichtVorangekommen = true;
		}

		// todo
	}
}

void OverwatcherClass::ErrorHandler(String Msg)
{
	if (Msg == "DrehFehler")
	{

		OW.drehFehlerCounter++;

		Serial.print(F("DrehFehler im Overwatcher erkannt. drehFehlerCounter:"));
		Serial.println(OW.drehFehlerCounter);

		if (OW.drehFehlerCounter >= 2)
		{
			OW.drehFehlerCounter = 0;
			Serial.println(F("Overwatcher.ErrorHandler(): Behebe Drehfehler."));

			OW.stateMachine->SendDirectCommand("bg-e");  // fahre einmal rückwärts vlt hilfts ja.
		}
		
	}
}
