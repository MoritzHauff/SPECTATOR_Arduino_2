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
	float mittelwert = 0.5 * (TempLinks + TempVorne);

	if (TempRechts > mittelwert + C_Opfer_TempUnterschied)
	{
		opferRechts++;
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
	if (opferLinks > 20 && letztesFeldOpferErkannt == false)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferVorne()
{
	if (opferVorne > 20 && letztesFeldOpferErkannt == false)
	{
		return true;
	}
	return false;
}

bool OpferKontrolleurClass::OpferRechts()
{
	if (opferRechts > 20 && letztesFeldOpferErkannt == false)
	{
		return true;
	}
	return false;
}
