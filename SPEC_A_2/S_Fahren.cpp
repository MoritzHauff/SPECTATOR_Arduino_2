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
	else
	{
		spectator->UpdateLaser();
	}

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	//spectator->UpdateHCSr04Seitlich(); // this should not be done always becaue the method is blocking.
	spectator->UpdateHCSr04VorneHinten();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_FahrenClass::Think()
{
	toggleState = !toggleState;
	/*Serial.print("ToggleState: ");
	Serial.println(toggleState);*/
}

void S_FahrenClass::Act()
{
	spectator->Motoren.SetMotoren(MotorSpeedL, MotorSpeedR);

	//Serial.println("Motorspeed gesetzt");

	spectator->HeartbeatLED.Toggle();
}
