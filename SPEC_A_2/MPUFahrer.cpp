/** MPUFahrer.cpp
***
*** Diese Klasse verwaltet die MPU Werte und berechnet anhand der aktuellen die n�tigen
*** Korrekturen um in eine bestimmte Richtung zu fahren oder zu drehen.
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

///////////////////////////////////////////////////////////////////////////
///Includes
#include "MPUFahrer.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MPUFahrerClass::MPUFahrerClass()
{
	orientierungswinkel[R_NORDEN] = 0;
	orientierungswinkel[R_OSTEN] = PI2;
	orientierungswinkel[R_SUEDEN] = PI;
	orientierungswinkel[R_WESTEN] = PI + PI2;

	zielRichtung = R_NORDEN;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
float MPUFahrerClass::GetWinkelAbstand(byte ZielRichtung, float aktYaw)
{
	float zielWinkel = orientierungswinkel[ZielRichtung];

	return minWinkelAbstand(aktYaw, zielWinkel);
}

void MPUFahrerClass::SetRichtungsWinkel(byte Richtung, float degree)
{
	orientierungswinkel[Richtung] = degree;
}

void MPUFahrerClass::SetNorden(float degree)
{
    if(degree < untereGrenze || degree > obereGrenze)
    {
        // todo: Fehlermeldung ausgeben
    }

    float winkel = degree;

    winkel = winkelvergroessern(winkel, 0); // auf 0 ... 2*Pi normieren.
    SetRichtungsWinkel(R_NORDEN, winkel);

    winkel = winkelvergroessern(winkel, PI2);
    SetRichtungsWinkel(R_OSTEN, winkel);

    winkel = winkelvergroessern(winkel, PI2);
    SetRichtungsWinkel(R_SUEDEN, winkel);

    winkel = winkelvergroessern(winkel, PI2);
    SetRichtungsWinkel(R_WESTEN, winkel);
}

float MPUFahrerClass::winkelverkleinern(float alterwinkel, float umwieviel)
{
	alterwinkel = alterwinkel - umwieviel;

	while (!(alterwinkel >= untereGrenze && alterwinkel < obereGrenze))
	{
		if (alterwinkel < untereGrenze)
		{
			alterwinkel = alterwinkel + obereGrenze;
		}
		else if (alterwinkel >= obereGrenze)
		{
			alterwinkel = alterwinkel - obereGrenze;
		}
	}

	return alterwinkel;
}

float MPUFahrerClass::winkelvergroessern(float alterwinkel, float umwieviel)
{
	return (winkelverkleinern(alterwinkel, -umwieviel));
}

float MPUFahrerClass::winkelAbstand(float value1, float value2)
{
	while (value1 >= obereGrenze)   // Winkel normieren
	{
		value1 -= obereGrenze;
	}
	while (value2 >= obereGrenze)
	{
		value2 -= obereGrenze;
	}


	if (value1 <= value2)
	{
		return value2 - value1;
	}
	else
	{
		value2 = value2 + obereGrenze;
		return value2 - value1;
	}
}

float MPUFahrerClass::minWinkelAbstand(float value1, float value2)
{
	float posAbstand = winkelAbstand(value1, value2);
	float negAbstand = winkelAbstand(value2, value1);

	if (posAbstand <= negAbstand)
	{
		return posAbstand;
	}

	return -negAbstand;
}

bool MPUFahrerClass::groesser(float value1, float value2, float abweichung)
{
	if (value1 > value2 && value1 < value2 + abweichung)
	{
		return true;
	}
	if (value2 + abweichung > obereGrenze)
	{
		if (value1 < untereGrenze + (value2 + abweichung) - obereGrenze)
		{
			return true;
		}
	}

	return false;
}

byte MPUFahrerClass::CharToRichtung(char c)
{
	if (c == 'n')
	{
		return R_NORDEN;
	}
	if (c == 'o')
	{
		return R_OSTEN;
	}
	if (c == 's')
	{
		return R_SUEDEN;
	}
	if (c == 'w')
	{
		return R_WESTEN;
	}
	else
	{
		// todo: Fehlerausgabe
		return 5;
	}
}

char MPUFahrerClass::RichtungToChar(byte Richtung)
{
	if (Richtung == R_NORDEN)
	{
		return 'n';
	}
	if (Richtung == R_OSTEN)
	{
		return 'o';
	}
	if (Richtung == R_SUEDEN)
	{
		return 's';
	}
	if (Richtung == R_WESTEN)
	{
		return 'w';
	}
	else
	{
		// todo: Fehlerausgabe
		return '5';
	}
}

byte MPUFahrerClass::CalculateRichtung(float aktYaw)
{
	byte richtung = R_NORDEN;
	byte minWinkel = abs(minWinkelAbstand(aktYaw, orientierungswinkel[richtung]));
	for (int i = 2; i <= R_WESTEN; i++)
	{
		byte hilfWinkel = abs(minWinkelAbstand(aktYaw, orientierungswinkel[i]));
		if (hilfWinkel < minWinkel)
		{
			minWinkel = hilfWinkel;
			richtung = i;
		}
	}

	if (minWinkel < MPUFahrer_CalculateRichtung_Toleranz)
	{
		return richtung;
	}
	else
	{
		Serial.println("FAILURE: MPUFahrer.CalculateRichtung(): Roboter steht gerade keiner Richtung zugeordnet. Es sollte zun�chst ein Drehbefehl ausgef�hrt werden.");
		return 0;
	}
}
