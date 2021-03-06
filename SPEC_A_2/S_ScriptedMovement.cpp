/** S_ScriptedMovement.cpp
***
*** Dieser State dient dem Abfahren eines vorgelegten Scripts (MovementArray)
*** welches die genauen Motorgeschwindigkeiten f�r jeden Tick beinhaltet. 
*** Dabei laufen nur die grundlegensten Zusammensto�verhinderungsma�nahmen.
***
*** Moritz Hauff, 27.04.2017
**/

///////////////////////////////////////////////////////////////////////////
/// Copyright (C) {2017}  {Moritz Hauff}
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// If you start using parts of this code please write a short message to: 
/// license@vierradroboter.de
///
/// If you have any questions contact me via mail: admin@vierradroboter.de
///////////////////////////////////////////////////////////////////////////

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

	counter = 0;
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

	counter++;

	// todo zusammensto�verhinderungen hinzuf�gen.
}

void S_ScriptedMovementClass::Act()
{
	MotorDaten *md = NULL;
	
	if (AusweichBewegung == BumperLinks)
	{
		//md = movementArrayHandler.GetNextMovement(L_bumperLinks, R_bumperLinks, bumperLinksRows);

		if (counter < 4)
		{
			md = new MotorDaten(-150, -100);
		}
		else if (counter < 12)
		{
			md = new MotorDaten(-150, 60);
		}
		else if (counter < 19)
		{
			md = new MotorDaten(-130, -130);
		}
		else
		{
			md = NULL;  // Finished
		}
	}
	else if (AusweichBewegung == BumperRechts)
	{
		if (counter < 4)
		{
			md = new MotorDaten(-100, -150);
		}
		else if (counter < 12)
		{
			md = new MotorDaten(60, -150);
		}
		else if (counter < 19)
		{
			md = new MotorDaten(-130, -130);
		}
		else
		{
			md = NULL;  // Finished
		}
	}
	else
	{
		Serial.println(F("S_ScriptedMovement.Act(): ERROR gewuenschte Ausweichbewegung nicht verf�gbar."));
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
