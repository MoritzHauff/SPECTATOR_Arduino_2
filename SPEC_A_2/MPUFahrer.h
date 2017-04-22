// MPUFahrer.h - Moritz Hauff - 16.03.2017

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

#ifndef _MPUFAHRER_h
#define _MPUFAHRER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///Constants
#define R_NORDEN 1
#define R_OSTEN 2
#define R_SUEDEN 3
#define R_WESTEN 4

#define PI 3.141592653589
#define PI2 1.5707963267945
#define ZWEIPI 6.283185307178

const float MPUFahrer_CalculateRichtung_Toleranz = 0.5; // Gibt die Toleranz an bis zu welchem Winkel eine Richtung noch als solche erkannt wird.

///////////////////////////////////////////////////////////////////////////
///MPUFahrer-Class
/*Diese Klasse verwaltet die MPU Werte und berechnet anhand der aktuellen die nötigen
Korrekturen um in eine bestimmte Richtung zu fahren oder zu drehen.*/
class MPUFahrerClass
{
 protected:
	 /// <summary>
	 /// Gibt den kleinst möglichen Winkel an. (Zur Berechnung, wann ein gesamter Kreis überstrichen wurde.)
	 /// </summary>
	 const float untereGrenze = 0;
	 /// <summary>
	 /// Gibt den größt möglichen Winkel an. (Zur Berechnung, wann ein gesamter Kreis überstrichen wurde.)
	 /// </summary>
	 const float obereGrenze = ZWEIPI;

	 /* Hier werden die den vier Himmelrichtungen entsprechenden Yaw-Winkel gespeichert.*/
	 float orientierungswinkel[5];

	 /* Gibt momentane Zielrichtung an (sowohl zum Drehen als auch zum Fahren).
	 dabei ist relativ zur anfangskalibiereung 'Norden': 1=Norden - 2=Osten - 3=Sueden - 4=Westen.*/
	 byte zielRichtung;

	/* Verringert den übergebenen Winkel um den gewünschten Wert und beachtet dabei dass ein Kreis geschlossen ist.
		 /// </summary>
		 /// <param name="alterwinkel">Der Winkel der verkleinert werden soll.</param>
		 /// <param name="umwieviel">Gibt an um wie viel der Winkel verkleinert werden soll.</param>*/
	 float winkelverkleinern(float alterwinkel, float umwieviel);

	 float winkelvergroessern(float alterwinkel, float umwieviel);
	 /* Gibt den im mathematisch positiven Abstand eines Winkels zu einem anderen aus.*/
	 float winkelAbstand(float value1, float value2);
	 /* Gibt den minimal winkelabstand und in welcher richtung sich er dieser BEfindet an
	 (positiv: links davon, negativ rechts von value1).*/
	 float minWinkelAbstand(float value1, float value2);
	 /* Gibt an ob der erste Winkel im Rahmen einer gewünschten Abweichung größer als der zweite Winkel ist.
	 Beachtet dabei, dass ein Kreis geschlossen ist.*/
	 bool groesser(float value1, float value2, float abweichung);

 public:
	 MPUFahrerClass();

	 /*Setzt den Ziel - Winkel für die gewünschte Richtung auf den angegebenn Wert.
	 Sollte nur in Ausnahmefällen einzeln aufgerufen werden.*/
	 void SetRichtungsWinkel(byte Richtung, float degree);
	 /*Setzt den übergeben Winkel als neues 'Norden' und passt alle anderen
	 Himmelsrichtungen daran an. Mit dieser Funktion sollte normalerweise kalibriert werden.*/
	 void SetNorden(float degree);

	 /*Gibt die Winkeldifferenz der aktuellen Fahrt zu gewünschten Richtung zurück.*/
	 float GetWinkelAbstand(byte ZielRichtung, float aktYaw);

	 /*Berechnet anhand des übergebenen Winkel die dazugehörige Himmelsrichtung.*/
	 byte CalculateRichtung(float aktYaw);


	 /*Konvertiert 'n', 'o', 's', 'w' in die entsprechende byte-Richtung für die Berechnunsfunktionen.*/
	 byte CharToRichtung(char c);
	 /*Konvertiert die entsprechende byte-Richtung für die Berechnunsfunktionen in 'n', 'o', 's', 'w'.*/
	 char RichtungToChar(byte Richtung);
};

#endif
