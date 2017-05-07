// OpferKontrolleur.h - Moritz Hauff - 23.04.2017

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
static const float C_Opfer_TempUnterschied = 2.0;  // 1.0 führt manchmal zu fehlerkennungen.
static const float C_Opfer_Temp = 24.8;  // Lieber etwas höher sonst werdne opfer auch durch die Wände erkannt.
static const int C_Opfer_ResetCounter = 2;

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
