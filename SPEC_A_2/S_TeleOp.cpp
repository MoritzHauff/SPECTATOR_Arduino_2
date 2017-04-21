/** S_TeleOp.cpp
***
*** Beschreibt den StandardFahrmodus.
***
*** Moritz Hauff, 18.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_TeleOp.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_TeleOpClass::Init()
{
	toggleState = false;
}

///////////////////////////////////////////////////////////////////////////
///Functions
void S_TeleOpClass::Sense()
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

	spectator->serialBuffer.Flush();  // alle Nachrichten auf einmal sind zu lang.

	spectator->UpdateLDR();

	spectator->UpdateSwitches();

	//spectator->UpdateHCSr04Seitlich(); // this should not be done always becaue the method is blocking.
	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateEncoder();

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();
}

void S_TeleOpClass::Think()
{
	toggleState = !toggleState;
	/*Serial.print("ToggleState: ");
	Serial.println(toggleState);*/
}

void S_TeleOpClass::Act()
{
	spectator->Motoren.SetMotoren(MotorSpeedL, MotorSpeedR);

	//Serial.println("Motorspeed gesetzt");

	spectator->HeartbeatLED.Toggle();
}
