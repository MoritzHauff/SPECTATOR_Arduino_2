// 
// todo
// 

#include "S_Fahren.h"

void S_FahrenClass::Init()
{
	toggleState = false;
}

void S_FahrenClass::Sense()
{
	spectator->UpdateSharp();

	if (toggleState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		spectator->UpdateMLX();
	}

	spectator->UpdateMPU();

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

	// todo: make heartbeat.
}
