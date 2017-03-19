// MPUFahrer.h - Moritz Hauff - 16.03.2017

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
#define R_NORDEN 0
#define R_OSTEN 1
#define R_SUEDEN 2
#define R_WESTEN 3

///////////////////////////////////////////////////////////////////////////
///MPUFahrer-Class
/*Diese Klasse verwaltet die MPU Werte und berechnet anhand der aktuellen die n�tigen 
Korrekturen um in eine bestimmte Richtung zu fahren oder zu drehen.*/
class MPUFahrerClass
{
 protected:
	 /// <summary>
	 /// Gibt den kleinst m�glichen Winkel an. (Zur Berechnung, wann ein gesamter Kreis �berstrichen wurde.)
	 /// </summary>
	 const float untereGrenze = 0;
	 /// <summary>
	 /// Gibt den gr��t m�glichen Winkel an. (Zur Berechnung, wann ein gesamter Kreis �berstrichen wurde.)
	 /// </summary>
	 const float obereGrenze = 2*PI;

	 /* Hier werden die den vier Himmelrichtungen entsprechenden Yaw-Winkel gespeichert.*/
	 float orientierungswinkel[4];

	 /* Gibt momentane Zielrichtung an (sowohl zum Drehen als auch zum Fahren).
	 dabei ist relativ zur anfangskalibiereung 'Norden': 1 Norden - 2 Osten - 3 Sueden - 4 Westen.*/
	 byte zielRichtung;

	/* Verringert den �bergebenen Winkel um den gew�nschten Wert und beachtet dabei dass ein Kreis geschlossen ist.
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
	 /* Gibt an ob der erste Winkel im Rahmen einer gew�nschten Abweichung gr��er als der zweite Winkel ist.
	 Beachtet dabei, dass ein Kreis geschlossen ist.*/
	 bool groesser(float value1, float value2, float abweichung);

 public:
	 MPUFahrerClass();

	 /*Setzt den Ziel - Winkel f�r die gew�nschte Richtung auf den angegebenn Wert.
	 Sollte nur in Ausnahmef�llen einzeln aufgerufen werden.*/
	 void SetRichtungsWinkel(byte Richtung, float degree);
	 /*Setzt den �bergeben Winkel als neues 'Norden' und passt alle anderen 
	 Himmelsrichtungen daran an. Mit dieser Funktion sollte normalerweise kalibriert werden.*/
	 void SetNorden(float degree);


	 void BerechneVorwaerts(byte ZielRichtung, float aktYaw, int *motorSpeedL, int *motorSpeedR);
	 bool BerechneDrehen(byte ZielRichtung, float aktYaw, int *motorSpeedL, int *motorSpeedR);
};

#endif
