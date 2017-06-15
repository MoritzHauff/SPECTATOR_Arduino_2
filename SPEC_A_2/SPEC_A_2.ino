/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino, überwacht und verarbeitet 
*** dessen Sensoren.
***
*** Für sämtliche Sensoren besteht mindestens eine Klasse zur Aufnahme der 
*** Daten und zur Speicherung. Diese sind in den Filtern "Header Files - Sensors"
*** und "Source Files - Sensors" geordnet.
*** Die allgemeine Steuerung der Handlungen und die Auswahl welche Sensoren 
*** ausgelesen werden sollen übernimmt eine StateMachine. "Header Files - States"
*** und "Source Files - States".
*** 
***
*** Es besteht die Möglichkeit einer serielle Übertragung aller Sensorwerte und 
*** Steuerung der Motoren an einen angeschlossen RaspberryPi oder andere PCs.
***
*** Moritz Hauff, 01.03.2017
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

// todo warum werden nachwievor zwei opfer angezeigt und versorgt?
// todo drehfunktion anpassen, damit diese nicht mehr stuckt -> erhöhe geschw im laufe der vergangenen Zeit
// todo states wieder mit error beenden, wenn diese Fehlgeschlagen sind und vom Overwathcer korrigeiren lassen. (z.B. auch GeradeSonstWieNichtVorangekommen senden)

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECTATORClass.h"
#include "Overwatcher.h"

#include "StateMachine.h"
 
///////////////////////////////////////////////////////////////////////////
///Variablen
unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

///////////////////////////////////////////////////////////////////////////
///Instanzen
StateMachineClass *stateMachine;

///////////////////////////////////////////////////////////////////////////
///SRAM-Funktion
/// <summary>
/// Diese Funktion gibt die Menge des aktuell unbenutzten SRAMs zurück. 
/// Für weitere Informationen siehe: https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory*/.
/// </summary>
int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

/// <summary>
/// Diese Funktion gibt die Menge des aktuell unbenutzen SRAMs über
/// die serielle Schnittstelle aus. Diese muss davor gegebenfalls gestartet werden.
/// </summary>
void printFreeRam()
{
	Serial.print("Unused RAM: ");
	Serial.println(freeRam(), DEC);
}

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{		
	SA.Init();
	
	stateMachine = new StateMachineClass(&SA);
	stateMachine->Init();	

	printFreeRam();

	OW.Init(stateMachine);

	printFreeRam();
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	
	// todo: StateMachine zeitlich ausmessen.
	stateMachine->DoAction();

	zwei = micros();

	/*Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife führt zu keinen Fifo-Overflows!
	Serial.println(" us.");*/

	eins = micros();
	
	// todo: Zeitmessung des Overwatchers
	OW.Control();

	/*Serial.print("Bisher durchgefuehrte Aktionen: ");
	Serial.println(OW.GetActions());*/

	zwei = micros();

	/*Serial.print("ToF-Zeit: ");    // Die Zeit auf serielle Daten zu überprüfen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");*/        // Das bedeutet die Analyse des seriellen Streams benötigt so sehr wenig Zeit.

	//delay(1000);
}
