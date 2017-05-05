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
static const float C_Opfer_TempUnterschied = 2.0;  // 1.0 f�hrt manchmal zu fehlerkennungen.
static const float C_Opfer_Temp = 24.8;  // Lieber etwas h�her sonst werdne opfer auch durch die W�nde erkannt.
static const int C_Opfer_ResetCounter = 2;

///////////////////////////////////////////////////////////////////////////
///OpferKontrolleur-Klasse
/*Diese Klasse kontrolliert st�ndig die Temperatur und gibt gebenfalls an, dass Opfervorhandne sind.*/
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
	/*Setzt den OpferStatus (z.B.) Beim Befahren des n�chsten Feldes zur�ck.*/
	void Reset();

	bool OpferLinks();
	bool OpferVorne();
	bool OpferRechts();
};

#endif
