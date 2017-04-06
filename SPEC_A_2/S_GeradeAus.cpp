/** S_GeradeAus.cpp
***
*** Dieser State überwacht das GeradeAusfahren.
***
*** Moritz Hauff, 06.04.2017
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
#include "S_GeradeAus.h"

void S_GeradeAusClass::Init()
{
	status = Running;
	
	startTime = millis();

	speedL = S_GeradeAus_NormalSpeed * Direction;
	speedR = S_GeradeAus_NormalSpeed * Direction;

	Serial.print("Fahre GeradeAus. Richtung: ");
	Serial.println(Direction);

	stoppWahrscheinlichkeit = 0;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_GeradeAusClass::Sense()
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

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();    // Alle Sensornachrichten auf einmal sind zu lang für den SerialBuffer.

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_GeradeAusClass::Think()
{
	toggleState = !toggleState;

	if (status == Running && startTime + S_GeradeAus_MaxTimer < millis())
	{
		speedL = 0;
		speedR = 0;
		status = Error;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung hat zu lange gedauert! MaxTimer abgelaufen.");
	}
	if (status == Running && startTime + S_GeradeAus_FeldTraversTimer < millis())
	{
		stoppWahrscheinlichkeit += 10;
	}

	if (status == Running && stoppWahrscheinlichkeit >= S_GeradeAus_MaxStoppWahrscheinlichkeit)
	{
		speedL = 0;
		speedR = 0;
		status = Finished;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung beendet.");
	}
}

void S_GeradeAusClass::Act()
{
	spectator->Motoren.SetMotoren(speedL, speedR);

	spectator->HeartbeatLED.Toggle();
}

void S_GeradeAusClass::ShiftTimers(unsigned long ShiftAmount)
{
	startTime += ShiftAmount;
}
