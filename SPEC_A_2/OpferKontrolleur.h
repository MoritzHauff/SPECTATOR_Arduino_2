// OpferKontrolleur.h - Moritz Hauff - 23.04.2017

#ifndef _OPFERKONTROLLEUR_h
#define _OPFERKONTROLLEUR_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///Konstanten
static const float C_Opfer_TempUnterschied = 1.0;
static const int C_Opfer_ResetCounter = 5;

///////////////////////////////////////////////////////////////////////////
///OpferKontrolleur-Klasse
/*Diese Klasse kontrolliert ständig die Temperatur und gibt gebenfalls an, dass Opfervorhandne sind.*/
class OpferKontrolleurClass
{
protected:
	int letztesFeldOpferErkannt;
	int opferLinks;
	int opferRechts;
	int opferVorne;

public:
	OpferKontrolleurClass();

	/*Kontrolliert die Temperaturen und aktualisiert gegebenfalls den OpferStatus.*/
	void Check(float TempLinks, float TmpVorne, float TmpRechts);
	/*Setzt den OpferStatus (z.B.) Beim Befahren des nächsten Feldes zurück.*/
	void Reset();

	bool OpferLinks();
	bool OpferVorne();
	bool OpferRechts();
};

#endif
