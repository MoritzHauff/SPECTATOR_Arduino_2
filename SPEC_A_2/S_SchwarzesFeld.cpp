/** S_SchwarzesFeld.cpp
***
*** Dieser State dient dem Verlassen eines schwarzen Feldes.
***
*** Moritz Hauff, 23.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_SchwarzesFeld.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_SchwarzesFeldClass::Init()
{
	status = Running;

	startTime = millis();

	speedL = -120;
	speedR = -120;
	encoderL = 0;
	encoderR = 0;

	stoppWahrscheinlichkeit = 0;

	//Serial.print("SchwarzesFeld.Init(): stoppWahrscheinlichkeit: ");
	//Serial.println(stoppWahrscheinlichkeit);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_SchwarzesFeldClass::Sense()
{
	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateLDR(); // Update this! otherwise the Overwatcher will never stop sending the 'bSFRe'-Command.
	
	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_SchwarzesFeldClass::Think()
{
	if (stoppWahrscheinlichkeit >= 10)   // todo move this to StateClass
	{
		stoppWahrscheinlichkeit -= 10;    // Jeden Tick verringert sich die Stoppwahrscheinlihckeit wieder falls es mal zu einem "Fehlalarm" gekommen ist.
	}
	else if (stoppWahrscheinlichkeit < 0)
	{
		stoppWahrscheinlichkeit = 0;  // War der letzte Tick noch dagegen zu stopnnen wird in jeden neuen Tick diese Entscheidung wieder neutral getroffen.
	}

	if (status == Running && startTime + S_SchwarzesFeld_MaxTimer < millis())
	{
		speedL = 0;
		speedR = 0;
		status = Finished;  // use Finished because then the program continues normally  // todo should an error message be send to the overwatcher?

		Serial.println("ERROR S_SchwarzesFeld.Think(): Schwarzes Feld Recover hat zu lange gedauert! MaxTimer abgelaufen.");
	}

	if (ExpectedNumberOfEncoderTicks <= 1)
	{
		ExpectedNumberOfEncoderTicks = 2;   // Es soll immer ein bisschen zurückgefahren werden.
	}

	// Encoder Daten überwachen
	encoderL += spectator->Motoren.GetEncoderInfoL().CountsSinceLastTick;
	encoderR += spectator->Motoren.GetEncoderInfoR().CountsSinceLastTick;

	if (abs(encoderL) > ExpectedNumberOfEncoderTicks || abs(encoderR) > ExpectedNumberOfEncoderTicks)
	{
		Serial.print("S_SchwarzesFeld.Think(): Erwartete Enoder Ticks erreicht: ");
		Serial.println(ExpectedNumberOfEncoderTicks);

		stoppWahrscheinlichkeit = 200;
	}
	/*if (spectator->ultraschallHinten.GetDistance() <= S_SchwarzesFeld_USHinten)
	{
		stoppWahrscheinlichkeit += 40;

		Serial.print("S_SchwarzesFeld.Think(): Ultraschall detektiert hinten ein Hindernis. Entfernung: ");
		Serial.println(spectator->ultraschallHinten.GetDistance());
	}*/

	if (stoppWahrscheinlichkeit > 100)
	{
		status = Finished;
		speedL = 0;
		speedR = 0;
	}
}

void S_SchwarzesFeldClass::Act()
{
	spectator->Motoren.SetMotoren(speedL, speedR);
}

void S_SchwarzesFeldClass::ShiftTimers(unsigned long ShiftAmount)
{
	startTime += ShiftAmount;
}
