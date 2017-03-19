/** MPUFahrer.cpp
***
*** Diese Klasse verwaltet die MPU Werte und berechnet anhand der aktuellen die nötigen
*** Korrekturen um in eine bestimmte Richtung zu fahren oder zu drehen.
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "MPUFahrer.h"

using namespace std;

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


	/*/// <summary>
	///
	/// </summary>
	/// <param name="GewuenschteRichtung"></param>
	/// <param name="AktYaw"></param>
	/// <param name="ToleranzWinkel">Ab welcher Toleranz die aktuelle Richtung gar nicht mehr korrigiert werden soll.</param>
	/// <param name="VerlangsamungsWinkel">Ab wann die aktuelle Richtung korrigiert nur noch wenig werden soll.</param>
	/// <param name="AnpassungsWinkel">Ab wann die aktuelle Richtung korrigiert werden soll.</param>
	/// <returns></returns>
	public Korrektur GetKorrektur(Richtung GewuenschteRichtung, int AktYaw, int ToleranzWinkel, int VerlangsamungsWinkel, int AnpassungsWinkel)
{
	//mit umweg über algo:
	int zielWinkel = orientierungswinkel[GewuenschteRichtung.Nummer];
	//zielWinkel = winkelverkleinern(zielWinkel, millis() / 5000);
	//ohne -||-
	//int zielWinkel = orientierungswinkel[_richtung.Nummer];

	if (AktYaw + ToleranzWinkel > zielWinkel && AktYaw - ToleranzWinkel < zielWinkel)
	{
		return Korrektur.KeineKorrektur;
	}

	if (groesser(AktYaw, zielWinkel, VerlangsamungsWinkel))
	{
		return Korrektur.WenigNachLinks;
	}
	if (groesser(AktYaw, zielWinkel, AnpassungsWinkel))
	{
		return Korrektur.WeitNachLinks;
	}

	if (groesser(zielWinkel, AktYaw, VerlangsamungsWinkel))  // mit dem Umdrehen der verglichenen Winkel entfällt eine mögliche kleiner Funktion.
	{
		return Korrektur.WenigNachRechts;
	}
	if (groesser(zielWinkel, AktYaw, AnpassungsWinkel))
	{
		return Korrektur.WeitNachRechts;
	}

	return Korrektur.WeitNachRechts;   // Passen gar keine Werte, fahre nach rechts, damit er vielleicht wieder in einen Wertebereich reinrutscht.
}*/

///////////////////////////////////////////////////////////////////////////
///Funktionen
bool MPUFahrerClass::BerechneDrehen(short ZielRichtung, float aktYaw, int *motorSpeedL, int *motorSpeedR)  // todo: notfall timer wenn er festhängt.   // todo: nur korrektur anweisungen zurückgeben (+/- Geschwindigkeit -> rechts/lnks drehen) einheitlich zu berechne vorwärts
{
	float zielWinkel = orientierungswinkel[ZielRichtung];

	float winkelAbstand = minWinkelAbstand(aktYaw, zielWinkel);
																																														//int winkelabstand = minWinkelAbstand(aktYaw, zielWinkel);
	int motorspeed = (int)(winkelAbstand * 360);   // todo: insert a convenient function
	motorspeed = min(motorspeed, 180); // cap at 180
	motorspeed = max(motorspeed, 60);  // below 60 the motors wont turn.

	if (abs(winkelAbstand) <= 0.1)  // stopp-toleranz
	{
		*motorSpeedL = 0;
		*motorSpeedR = 0;

		return true; // true wenn drehen abgeschlossen
	}
	/*else if (winkelabstand < 0)
	{
		*motorSpeedL = -motorspeed;
		*motorSpeedR = motorspeed;
	}
	else if (winkelabstand > 0)
	{
		*motorSpeedL = motorspeed;
		*motorSpeedR = -motorspeed;   // should be done automaticall in minWinkelAbstand
	}*/
	else
	{
		*motorSpeedL = motorspeed;
		*motorSpeedR = -motorspeed;
		//Serial.println("Sth went wrong in the MPUFahrer!");
	}

	return false;
}


void MPUFahrerClass::BerechneVorwaerts(short ZielRichtung, float aktYaw, int *motorSpeedL, int *motorSpeedR)
{
	/*Korrektur aktKor = GetKorrektur(ZielRichtung, aktYaw, 3, 20, 85);   // todo: sinnvolle Geradeausfahr implementation

	if (aktKor == Korrektur.KeineKorrektur)
	{
		return new MotorDaten(250, 250);
	}
	if (aktKor == Korrektur.WeitNachRechts || aktKor == Korrektur.WenigNachRechts)
	{
		return new MotorDaten(250, 150);
	}
	if (aktKor == Korrektur.WeitNachLinks || aktKor == Korrektur.WenigNachLinks)
	{
		return new MotorDaten(150, 250);
	}*/
}

void MPUFahrerClass::SetRichtungsWinkel(short Richtung, float degree)
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

short MPUFahrerClass::CharToRichtung(char c)
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
