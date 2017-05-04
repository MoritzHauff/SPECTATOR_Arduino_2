/** S_ScriptedMovement.cpp
***
*** Dieser State dient dem Abfahren eines vorgelegten Scripts (MovementArray)
*** welches die genauen Motorgeschwindigkeiten für jeden Tick beinhaltet. 
*** Dabei laufen nur die grundlegensten Zusammenstoßverhinderungsmaßnahmen.
***
*** Moritz Hauff, 27.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_ScriptedMovement.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_ScriptedMovementClass::Init()
{
	movementArrayHandler.GoToStart();
	startTime = millis();

	status = Running;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_ScriptedMovementClass::Sense()
{
	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateLDR(); // Update this! otherwise the Overwatcher will never stop sending the 'bSFRe'-Command.

	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_ScriptedMovementClass::Think()
{
	if (startTime + S_ScriptedMovement_MaxTime < millis() && status == Running)
	{
		status = Error;

		Serial.println("FAILURE: Ausweichmanoever konnte nicht beendet werden. MaxTimer erreicht!");
	}

	// todo zusammenstoßverhinderungen hinzufügen.
}

void S_ScriptedMovementClass::Act()
{
	MotorDaten *md = NULL;
	
	if (AusweichBewegung == BumperLinks)
	{
		md = movementArrayHandler.GetNextMovement(L_bumperLinks, R_bumperLinks, bumperLinksRows);
	}
	else if (AusweichBewegung == BumperRechts)
	{
		// todo add
	}
	else
	{
		Serial.println(F("S_ScriptedMovement.Act(): ERROR gewuenschte Ausweichbewegung nicht verfügbar."));
		md = new MotorDaten(0, 0);
		status = Error;
	}

	if (md != NULL)
	{
		spectator->Motoren.SetMotoren(md->MotorSpeedL, md->MotorSpeedR);

		Serial.print("Weiche aus: L: ");
		Serial.print(md->MotorSpeedL);
		Serial.print(" R: ");
		Serial.println(md->MotorSpeedR);

		delay(50); // add a small delay.
	}
	else
	{
		spectator->mpu.ResetFIFO();
		status = Finished;
		spectator->Motoren.SetMotoren(0, 0);
	}
}

void S_ScriptedMovementClass::ShiftTimers(unsigned long ShiftAmount)
{
	startTime += ShiftAmount;
}
