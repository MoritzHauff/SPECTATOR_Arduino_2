/** Overwatcher.cpp
***
*** Diese Klasse kontrolliert ständig die Temperatur und gibt gebenfalls an, dass Opfervorhandne sind.
*** 
*** Moritz Hauff, 23.04.2017
**/

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
	if (letztesFeldOpferErkannt > 0)
	{
		letztesFeldOpferErkannt--;
	}

	if (OpferLinks() == true)
	{
		letztesFeldOpferErkannt = C_Opfer_ResetCounter;   // Falls auf dem letzten Feld ein Opfer erkannt wurde dies hier speichern.
	}
	if (OpferVorne() == true)
	{
		letztesFeldOpferErkannt = C_Opfer_ResetCounter;
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
	if (opferLinks > 3 && letztesFeldOpferErkannt <= 0)  // 5 ist vlt zu viel (fährt zu schnell)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferVorne()
{
	if (opferVorne > 3 && letztesFeldOpferErkannt <= 0)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferRechts()
{
	if (opferRechts > 3 && letztesFeldOpferErkannt <= 0)
	{
		return true;
	}
	return false;
}
