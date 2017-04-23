/** Overwatcher.cpp
***
*** Diese Klasse kontrolliert ständig die Temperatur und gibt gebenfalls an, dass Opfervorhandne sind.
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

///////////////////////////////////////////////////////////////////////////
///Includes
#include "OpferKontrolleur.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
OpferKontrolleurClass::OpferKontrolleurClass()
{
	Reset();
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void OpferKontrolleurClass::Reset()
{
	letztesFeldOpferErkannt = false;

	if (OpferLinks() == true)
	{
		letztesFeldOpferErkannt = true;   // Falls auf dem letzten Feld ein Opfer erkannt wurde dies hier speichern.
	}
	if (OpferVorne() == true)
	{
		letztesFeldOpferErkannt = true;
	}
	if (OpferRechts() == true)
	{
		letztesFeldOpferErkannt = true;
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
	float mittelwert = 0.5 * (TempVorne + TempRechts);

	if (TempLinks > mittelwert + C_Opfer_TempUnterschied)
	{
		opferLinks++;
		Serial.println("OpferLinks erkannt.");
	}

	mittelwert = 0.5 * (TempLinks + TempRechts);
	if (TempVorne > mittelwert + C_Opfer_TempUnterschied)
	{
		opferVorne++;
	}

	mittelwert = 0.5 * (TempLinks + TempVorne);
	if (TempRechts > mittelwert + C_Opfer_TempUnterschied)
	{
		opferRechts++;
	}
}

bool OpferKontrolleurClass::OpferLinks()
{
	if (opferLinks > 5 && letztesFeldOpferErkannt == false)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferVorne()
{
	if (opferVorne > 5 && letztesFeldOpferErkannt == false)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferRechts()
{
	if (opferRechts > 5 && letztesFeldOpferErkannt == false)
	{
		return true;
	}
	return false;
}
