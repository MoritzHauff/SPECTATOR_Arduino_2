/** S_Rampe.cpp
***
*** Dieser State dient dem Befahren der Rampe.
***
*** Moritz Hauff, 23.04.2017
**/

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

	NormalSpeed = S_Rampe_NormalSpeed;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_RampeClass::Sense()
{
	spectator->UpdateSharp();

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
		speedL = NormalSpeed + 10;
		speedR = NormalSpeed - 45;
	}
	else if (winkelKorrektur < -0.01)
	{
		speedL = NormalSpeed - 45;
		speedR = NormalSpeed + 10;
	}
	else
	{
		speedL = NormalSpeed;
		speedR = NormalSpeed;
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

	
	// Neigung kontrollieren
	if (abs(spectator->mpu.GetPitch()) < 0.2)
	{
		stoppWahrscheinlichkeit += 11;  // 17 ist zu hoch

		Serial.println("S_Rampe.Think(): MPU verkuendet Ende der Rampe.");
	}
	// Ultraschall kontrollieren.
	else if (spectator->ultraschallVorne.GetDistance() <= S_Rampe_USVorne)
	{
		stoppWahrscheinlichkeit += 60;

		Serial.print("S_Rampe.Think(): Ultraschall detektiert vorne ein Hindernis. Entfernung: ");
		Serial.println(spectator->ultraschallVorne.GetDistance());
	}

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
