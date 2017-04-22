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
	startTime = millis();

	speedL = -120;
	speedR = -120;
	encoderL = 0;
	encoderR = 0;

	stoppWahrscheinlichkeit = 0;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_SchwarzesFeldClass::Sense()
{
	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	spectator->UpdateHCSr04VorneHinten();
	
	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_SchwarzesFeldClass::Think()
{
	if (status == Running && startTime + S_SchwarzesFeld_MaxTimer < millis())
	{
		speedL = 0;
		speedR = 0;
		status = Error;

		Serial.println("ERROR S_SchwarzesFeld.Think(): Schwarzes Feld Recover hat zu lange gedauert! MaxTimer abgelaufen.");
	}

	// Encoder Daten überwachen
	encoderL += spectator->Motoren.GetEncoderInfoL().CountsSinceLastTick;
	encoderR += spectator->Motoren.GetEncoderInfoR().CountsSinceLastTick;

	if (abs(encoderL) > ExpectedNumberOfEncoderTicks || abs(encoderR) > ExpectedNumberOfEncoderTicks)
	{
		Serial.println("S_SchwarzesFeld.Think(): Erwartete Enoder Ticks erreicht.");
		
		stoppWahrscheinlichkeit = 200;
	}
	if (spectator->ultraschallHinten.GetDistance() <= S_SchwarzesFeld_USHinten)
	{
		stoppWahrscheinlichkeit += 60;

		Serial.print("S_SchwarzesFeld.Think(): Ultraschall detektiert hinten ein Hindernis. Entfernung: ");
		Serial.println(spectator->ultraschallVorne.GetDistance());
	}

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
