/** S_GeradeAus.cpp
***
*** Dieser State überwacht das GeradeAusfahren.
***
*** Moritz Hauff, 06.04.2017
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
#include "S_GeradeAus.h"

void S_GeradeAusClass::Init()
{
	status = Running;

	stoppWahrscheinlichkeit = 0;
	winkelKorrektur = 0;

	speedL = S_GeradeAus_NormalSpeed * Direction;
	speedR = S_GeradeAus_NormalSpeed * Direction;

	toggleState = false;  // Am anfang soll der Laser befragt werden.

	Sense();

	startRichtung = spectator->mpuFahrer.CalculateRichtung(spectator->mpu.GetYaw());
	startDistanceLaserV = spectator->laserVorne.GetDistance();
	startDistanceUSV = spectator->ultraschallVorne.GetDistance();
	startDistanceUSH = spectator->ultraschallHinten.GetDistance();

	startWandKategorie = ermittleStartWandKategorie(startDistanceLaserV);
	zielWandKategorie = startWandKategorie - Direction;   // todo: Achtung dass kann hier NULL werden!!!!

	Serial.print("Fahre GeradeAus. Fahrtrichtung: ");
	Serial.println(Direction);

	Serial.print("LaserDistanz: ");
	Serial.print(startDistanceLaserV); 
	Serial.print(" Kategorie: ");
	Serial.println(startWandKategorie);
	Serial.print("ZielKategorie: ");
	Serial.print(zielWandKategorie);
	Serial.print(" Zielentfernung: ");
	Serial.print(S_GeradeAus_WandErntfernungen[zielWandKategorie]);
	Serial.print("Ultraschall Distanz Vorne: ");
	Serial.println(startDistanceUSV);
	Serial.print("Ultraschall Distanz Hinten: ");
	Serial.println(startDistanceUSH);

	startTime = millis();
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_GeradeAusClass::Sense()
{
	spectator->UpdateSharp();

	if (toggleState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		spectator->UpdateMLX();
	}
	else
	{
		//spectator->UpdateLaser();
	}

	spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();    // Alle Sensornachrichten auf einmal sind zu lang für den SerialBuffer.

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_GeradeAusClass::Think()
{
	toggleState = !toggleState;

	// Winkel korrektur ermitteln
	winkelKorrektur = spectator->mpuFahrer.BerechneVorwaerts(startRichtung, spectator->mpu.GetYaw());

	int h = winkelKorrektur * S_GeradeAus_WinkelRatio;

	/*Serial.print("momentan berechnete (MPU) Winkelkorrektur: ");
	Serial.println(winkelKorrektur);
	Serial.print("berechnete Korrekturmasnahme: ");
	Serial.print(h);*/

	adaptedSpeedL = capSpeed(speedL + h * Direction, S_GeradeAus_NormalSpeed, 70);
	adaptedSpeedR = capSpeed(speedR - h * Direction, S_GeradeAus_NormalSpeed, 70);

	//Stopp zeitpunkt ermitteln
	if (status == Running && startTime + S_GeradeAus_MaxTimer < millis())
	{
		speedL = 0;
		speedR = 0;
		status = Error;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung hat zu lange gedauert! MaxTimer abgelaufen.");
	}
	if (status == Running && startTime + S_GeradeAus_FeldTraversTimer < millis())
	{
		stoppWahrscheinlichkeit += 10;
	}

	if (status == Running && stoppWahrscheinlichkeit >= S_GeradeAus_MaxStoppWahrscheinlichkeit)
	{
		speedL = 0;
		speedR = 0;
		status = Finished;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung beendet.");
	}

	if (Direction == 1)
	{
		if (spectator->laserVorne.GetDistance() < S_GeradeAus_WandErntfernungen[zielWandKategorie] + S_GeradeAus_WandEntfernungsKorrektur)
		{
			speedL = 0;
			speedR = 0;
			status = Finished;

			Serial.print("S_GeradeAus.Think(): Felddurchquerung aufgrund des Lasers beendet. Entfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
	}
	if (Direction == -1)
	{
		if (spectator->laserVorne.GetDistance() > S_GeradeAus_WandErntfernungen[zielWandKategorie] - S_GeradeAus_WandEntfernungsKorrektur)
		{
			speedL = 0;
			speedR = 0;
			status = Finished;

			Serial.print("S_GeradeAus.Think(): Felddurchquerung aufgrund des Lasers beendet. Entfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
	}
}

void S_GeradeAusClass::Act()
{
	//spectator->Motoren.SetMotoren(adaptedSpeedL, adaptedSpeedR);
	spectator->Motoren.SetMotoren(speedL, speedR);

	spectator->HeartbeatLED.Toggle();
}

void S_GeradeAusClass::ShiftTimers(unsigned long ShiftAmount)
{
	startTime += ShiftAmount;
}

int S_GeradeAusClass::capSpeed(int Value, int Upper, int Lower)
{
	if (Value > 0)
	{
		Value = min(Value, Upper);
		Value = max(Value, Lower);
	}
	else
	{
		Value = min(Value, -Lower);
		Value = max(Value, -Upper);
	}

	return Value;
}

byte S_GeradeAusClass::ermittleStartWandKategorie(int StartDistance)
{
	int minAbstand = abs(StartDistance - S_GeradeAus_WandErntfernungen[0]);
	byte b = 0;
	for (byte i = 1; i < sizeof(S_GeradeAus_WandErntfernungen); i++)
	{
		int h = abs(StartDistance - S_GeradeAus_WandErntfernungen[i]);
		if (h < minAbstand)
		{
			minAbstand = h;
			b = i;
		}
	}

	return b;
}