/** S_Rampe.cpp
***
*** Dieser State dient dem Befahren der Rampe.
***
*** Moritz Hauff, 23.04.2017
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
#include "S_Rampe.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_RampeClass::Init()
{
	status = Running;   // todo move this to StateClass

	speedL = S_Rampe_NormalSpeed;
	speedR = S_Rampe_NormalSpeed;

	// Himmelsrichtung ermitteln.
	startRichtung = spectator->mpuFahrer.CalculateRichtung(spectator->mpu.GetYaw());
	winkelKorrektur = spectator->mpuFahrer.GetWinkelAbstand(startRichtung, spectator->mpu.GetYaw());
	
	stoppWahrscheinlichkeit = 0;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_RampeClass::Sense()
{
	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateLDR();

	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_RampeClass::Think()
{
	if (stoppWahrscheinlichkeit >= 10)
	{
		stoppWahrscheinlichkeit -= 10;    // Jeden Tick verringert sich die Stoppwahrscheinlihckeit wieder falls es mal zu einem "Fehlalarm" gekommen ist.
	}
	else if (stoppWahrscheinlichkeit < 0)
	{
		stoppWahrscheinlichkeit = 0;  // War der letzte Tick noch dagegen zu stopnnen wird in jeden neuen Tick diese Entscheidung wieder neutral getroffen.
	}

	// Winkelkorrektur ermitteln
	winkelKorrektur = spectator->mpuFahrer.GetWinkelAbstand(startRichtung, spectator->mpu.GetYaw());

	winkelKorrektur += -verwerteSharp(spectator->sharplinksvorne.GetValue(), spectator->sharplinkshinten.GetValue());
	winkelKorrektur += verwerteSharp(spectator->sharprechtsvorne.GetValue(), spectator->sharprechtshinten.GetValue());

	if (winkelKorrektur > 0.01)
	{
		speedL = S_Rampe_NormalSpeed + 10;
		speedR = S_Rampe_NormalSpeed - 45;
	}
	else if (winkelKorrektur < -0.01)
	{
		speedL = S_Rampe_NormalSpeed - 45;
		speedR = S_Rampe_NormalSpeed + 10;
	}
	else
	{
		speedL = S_Rampe_NormalSpeed;
		speedR = S_Rampe_NormalSpeed;
	}


	// Bumper kontrollieren.
	if (spectator->switchLinks.GetLastState() == true)
	{
		stoppWahrscheinlichkeit += 30;
		Serial.println("WARNING S_Rampe: linker Bumper hat Kontakt.");
	}
	if (spectator->switchRechts.GetLastState() == true)
	{
		stoppWahrscheinlichkeit += 30;
		Serial.println("WARNING S_Rampe: rechter Bumper hat Kontakt.");
	}

	// Ultraschall kontrollieren.
	if (spectator->ultraschallVorne.GetDistance() <= S_Rampe_USVorne)
	{
		stoppWahrscheinlichkeit += 60;

		Serial.print("S_Rampe.Think(): Ultraschall detektiert vorne ein Hindernis. Entfernung: ");
		Serial.println(spectator->ultraschallVorne.GetDistance());
	}
	/*// Neigung kontrollieren
	if (abs(spectator->mpu.GetPitch()) < 0.2)
	{
		stoppWahrscheinlichkeit += 11;  // 17 ist zu hoch

		Serial.println("S_Rampe.Think(): MPU verkuendet Ende der Rampe.");
	}*/

	if (stoppWahrscheinlichkeit > 100)
	{
		status = Finished;
		speedL = 0;
		speedR = 0;
	}
}

/*Gibt eine Winkelkorrektur an. Pos: zur Wand hin, neg von der jeweiligen Wand weg.*/
float S_RampeClass::verwerteSharp(int Vorne, int Hinten)  // todo move this somewhere else (not copy from S_GeradeAus)
{
	if (Vorne < 150 || Hinten < 150)  // zu weit weg von den Wänden, keine Korrektur möglich
	{
		return 0;
	}
	else if (Vorne < 280 || Hinten < 280)  // zu weit aber in korrigierabstand.
	{
		return 0.1;
	}
	else if (Vorne > 640 || Hinten > 640)  // zu nah an der Wand drehe davon weg.
	{
		//turnCounter = 5;
		//straightCounter = 5;
		Serial.println("S_Rampe.Think(): Sharp melden deutliche Naehe zur Wand, korrigiere...");
		return -0.15;
	}
	else if (Vorne > 420 || Hinten > 420)   // zu nah aber noch nicht so nah das gerade aus gefahren werden muss.
	{
		Serial.println("S_Rampe.Think(): Sharp melden etwas Naehe zur Wand, korrigiere...");
		return -0.07;
	}
	else
	{
		int differenz = Hinten - Vorne;  // positiv wenn hinten näher an wand als vorne.

		if (abs(differenz) < 25)
		{
			return 0;
		}
		else if (differenz > 0)
		{
			Serial.println("S_Rampe.Think(): Sharp verlangen richtungskorrektur zur Wand hin.");
			return 0.05;
		}
		else if (differenz < 0)
		{
			Serial.println("S_Rampe.Think(): Sharp verlangen richtungskorrektur von Wand weg.");
			return -0.05;
		}

		return 0;
	}
}

void S_RampeClass::Act()
{
	spectator->Motoren.SetMotoren(speedL, speedR);
}

void S_RampeClass::ShiftTimers(unsigned long ShiftAmount)
{
	// do nothing
}
