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
bool MPUFahrerClass::BerechneDrehen(byte ZielRichtung, float aktYaw, int *motorSpeedL, int *motorSpeedR)  // todo: notfall timer wenn er festhängt.   // todo: nur korrektur anweisungen zurückgeben (+/- Geschwindigkeit -> rechts/lnks drehen) einheitlich zu berechne vorwärts
{
	float zielWinkel = orientierungswinkel[ZielRichtung];

	float winkelAbstand = minWinkelAbstand(aktYaw, zielWinkel);

	if (abs(winkelAbstand) <= 0.01)  // stopp-toleranz   // 0.1 = 5,7°
	{
		*motorSpeedL = 0;
		*motorSpeedR = 0;

		return true; // true wenn drehen abgeschlossen
	}
																																														//int winkelabstand = minWinkelAbstand(aktYaw, zielWinkel);
	int motorspeed = (int)(winkelAbstand * 400);   // todo: insert a convenient function   // 360
	if(motorspeed > 0)
    {
        motorspeed = min(motorspeed, 180); // cap at 180
        motorspeed = max(motorspeed, 70);  // below 60 the motors wont turn.
    }
	if(motorspeed < 0)
    {
        motorspeed = max(motorspeed, -180); // cap at 180
        motorspeed = min(motorspeed, -70);  // below 60 the motors wont turn.   // todo cap mit der zeit erhöhen wenn er sich nicht mehr dreht.
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

    *motorSpeedL = motorspeed;
    *motorSpeedR = -motorspeed;

	return false;
}


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
		Serial.println("FAILURE: MPUFahrer.CalculateRichtung(): Roboter steht gerade keiner Richtung zugeordnet. Es sollte zunächst ein Drehbefehl ausgeführt werden.");
		return 0;
	}
}