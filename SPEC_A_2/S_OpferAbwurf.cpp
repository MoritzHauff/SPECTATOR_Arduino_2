/** S_OpferAbwurf.cpp
***
*** Dieser State überwacht den Abwurf von RescueKits.
***
*** Moritz Hauff, 04.05.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_OpferAbwurf.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_OpferAbwurfClass::Init()
{
	// Variablen zurücksetzen.
	status = Running;
	dropped = false;

}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_OpferAbwurfClass::Sense()
{
	// do nothing
}

void S_OpferAbwurfClass::Think()
{
	if (dropped == true)
	{
		spectator->mpu.ResetFIFO();
		status = Finished;
	}
}

void S_OpferAbwurfClass::Act()
{
	if (dropped == false)
	{
		dropped = true;

		if (RechtsLinks == 'l')
		{
			spectator->Motoren.AbwurfLinks();
		}
		else if (RechtsLinks == 'r')
		{
			spectator->Motoren.AbwurfRechts();
		}
		else
		{
			Serial.println(F("S_OpferAbwurfClass.Act(): Falsche Opferabwurfrichtung angegeben. Nur 'r' und 'l' moeglich!"));
			spectator->Motoren.AbwurfLinks();
		}
	}
}

void S_OpferAbwurfClass::ShiftTimers(unsigned long ShiftAmount)
{
	// do nothing.
}
