/** S_Fahren.cpp
***
*** Beschreibt den StandardFahrmodus.
***
*** Moritz Hauff, 18.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_Fahren.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_FahrenClass::Init()
{
	toggleState = false;
}

///////////////////////////////////////////////////////////////////////////
///Functions
void S_FahrenClass::Sense()
{
	spectator->UpdateSharp();

	if (toggleState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		spectator->UpdateMLX();
	}

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();
}

void S_FahrenClass::Think()
{
	// do nothing

	toggleState = !toggleState;
}

void S_FahrenClass::Act()
{
	spectator->Motoren.SetMotoren(spectator->MotorSpeedL, spectator->MotorSpeedR);

	//Serial.println("Motorspeed gesetzt");

	spectator->HeartbeatLED.Toggle();
}
