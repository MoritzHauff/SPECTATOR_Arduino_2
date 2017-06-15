/** Overwatcher.cpp
***
*** Diese Klasse kontrolliert ständig die Temperatur und gibt gebenfalls an, 
*** dass Opfer vorhanden sind.
*** 
*** Moritz Hauff, 23.04.2017
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

// todo bumper rückwärts fahren 
// todo felder hierachiiesieren (bumperfelder am bestern vermeiden)
// todo checkpoints erkennen und beim zurücksetzen merken
// todo beim zurücksetzen fahrmodus wechslen (geradeaus / rechte hand)
// todo beim zurücksetzen kein totalreset der alten anfangsfelder damit er dann in eine neue richtung fährt. ( muss in anfangsrichtung ausgerichtet werden.)

///////////////////////////////////////////////////////////////////////////
///Includes
#include "OpferKontrolleur.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
OpferKontrolleurClass::OpferKontrolleurClass()
{
	opferLinks = 0;
	opferRechts = 0;
	opferVorne = 0;
	Reset();
	letztesFeldOpferErkannt = -3;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void OpferKontrolleurClass::Reset()
{
	if (letztesFeldOpferErkannt > 0)
	{
		letztesFeldOpferErkannt--;
	}

	if (OpferLinks() == true)
	{
		letztesFeldOpferErkannt = C_Opfer_ResetCounter;   // Falls auf dem letzten Feld ein Opfer erkannt wurde dies hier speichern.
		Serial.println(F("OpferKontrolleur.Reset(): Habe auf dem letzten Feld ein Opfer erkannt."));
	}
	if (OpferVorne() == true)
	{
		letztesFeldOpferErkannt = C_Opfer_ResetCounter;
		Serial.println(F("OpferKontrolleur.Reset(): Habe auf dem letzten Feld ein Opfer erkannt."));
	}
	if (OpferRechts() == true)
	{
		letztesFeldOpferErkannt = C_Opfer_ResetCounter;
		Serial.println(F("OpferKontrolleur.Reset(): Habe auf dem letzten Feld ein Opfer erkannt."));
	}

	opferLinks = 0;
	opferVorne = 0;
	opferRechts = 0;
}

void OpferKontrolleurClass::Check(float TempLinks, float TempVorne, float TempRechts)
{
	/*Serial.println("OpferCheck: ");
	Serial.print("\tLinks: ");
	Serial.print(TempLinks);
	Serial.print("\tVorne: ");
	Serial.print(TempVorne);
	Serial.print("\tRechts");
	Serial.println(TempRechts);*/
	//float mittelwert = 0.5 * (TempVorne + TempRechts);

	//if (TempLinks > mittelwert + C_Opfer_TempUnterschied)
	if (TempLinks > C_Opfer_Temp)
	{
		opferLinks++;
		Serial.println("OpferLinks erkannt.");
	}

	//mittelwert = 0.5 * (TempLinks + TempRechts);
	//if (TempVorne > mittelwert + C_Opfer_TempUnterschied)
	if (TempVorne > C_Opfer_Temp)
	{
		opferVorne++;
		Serial.println("Opfer Vorne erkannt.");
	}

	//mittelwert = 0.5 * (TempLinks + TempVorne);
	//if (TempRechts > mittelwert + C_Opfer_TempUnterschied)
	if (TempRechts > C_Opfer_Temp)
	{
		opferRechts++;
		Serial.println("OpferRechts erkannt.");
	}
}

bool OpferKontrolleurClass::OpferLinks()
{
	if (opferLinks >= 2 && letztesFeldOpferErkannt <= 0)  // 5 ist vlt zu viel (fährt zu schnell)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferVorne()
{
	if (opferVorne >= 2 && letztesFeldOpferErkannt <= 0)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferRechts()
{
	if (opferRechts >= 2 && letztesFeldOpferErkannt <= 0)
	{
		return true;
	}
	return false;
}
