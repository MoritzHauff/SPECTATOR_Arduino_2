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

	spectator->serialBuffer.AddMsg(C_WandVorne, spectator->GetWand(R_VORNE));
	spectator->serialBuffer.AddMsg(C_WandRechts, spectator->GetWand(R_RECHTS));
	spectator->serialBuffer.AddMsg(C_WandHinten, spectator->GetWand(R_HINTEN));
	spectator->serialBuffer.AddMsg(C_WandLinks, spectator->GetWand(R_LINKS));
	spectator->serialBuffer.AddMsg(C_OpferLinks, false);
	spectator->serialBuffer.AddMsg(C_OpferVorne, false);
	spectator->serialBuffer.AddMsg(C_OpferRechts, false);
	spectator->serialBuffer.Flush();

	spectator->serialBuffer.AddMsg(C_AktuelleRichtung, spectator->AktRichtung);
	spectator->serialBuffer.AddMsg(C_GeradeRampeBefahren, spectator->GeradeRampeBefahren);
	spectator->serialBuffer.AddMsg(C_GeradeSchwarzesFeldBefahren, spectator->GeradeSchwarzesFeldBefahren);
	spectator->serialBuffer.Flush();

	Serial.println("DemandNextStep");
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_IdleClass::Sense()
{
	//spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateLDR();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
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
