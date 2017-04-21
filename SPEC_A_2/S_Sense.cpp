/** S_Sense.cpp
***
*** Dieser State erfasst nach jeder Aktion das aktuelle Feld und 
*** beendet sich nach 10 Ticks wieder.
***
*** Moritz Hauff, 21.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_Sense.h"

void S_SenseClass::Init()
{
	// Variablen zurücksetzen.
	status = Running;
	counter = 0;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_SenseClass::Sense()
{
	spectator->UpdateSharp();

	spectator->UpdateMLX();

	spectator->serialBuffer.Flush();  // alle Nachrichten auf einmal sind zu lang.

	spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateLDR();

	spectator->serialBuffer.Flush();  // alle Nachrichten auf einmal sind zu lang.

	spectator->UpdateSwitches();

	spectator->UpdateHCSr04Seitlich(); // this should not be done always becaue the method is blocking.

	spectator->UpdateHCSr04VorneHinten();

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();
}

void S_SenseClass::Think()
{
	counter++;
	if (counter > S_Sense_NumberOfTicks)
	{
		status = Finished;
	}
}

void S_SenseClass::Act()
{
	spectator->Motoren.SetMotoren(0, 0);  // Stop any movement.
}

void S_SenseClass::ShiftTimers(unsigned long ShiftAmount)
{
	// do nothing.
}
