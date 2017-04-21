/** S_Sense.cpp
***
*** Dieser State erfasst nach jeder Aktion das aktuelle Feld und
*** beendet sich nach 5 Ticks wieder.
***
*** Moritz Hauff, 21.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_Idle.h"

void S_IdleClass::Init()
{
	// Variablen zurücksetzen.
	status = Running;
	counter = 0;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_IdleClass::Sense()
{
	//spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateLDR();

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();
}

void S_IdleClass::Think()
{
	counter++;
	if (counter > S_Idle_NumberOfTicks)
	{
		status = Finished;
	}
}

void S_IdleClass::Act()
{
	spectator->Motoren.SetMotoren(0, 0);  // Stop any movement.
}

void S_IdleClass::ShiftTimers(unsigned long ShiftAmount)
{
	// do nothing.
}
