/** S_Sense.cpp
***
*** Dieser State erfasst nach jeder Aktion das aktuelle Feld und 
*** beendet sich nach 5 Ticks wieder.
***
*** Moritz Hauff, 21.04.2017
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
	spectator->OpferKontroller.Check(spectator->MLXLinks.GetObjTemp(), spectator->MLXVorne.GetObjTemp(), spectator->MLXRechts.GetObjTemp());

	//spectator->serialBuffer.Flush();  // alle Nachrichten auf einmal sind zu lang.
	spectator->serialBuffer.Clear();

	//spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateLDR();

	//spectator->serialBuffer.Flush();  // alle Nachrichten auf einmal sind zu lang.
	spectator->serialBuffer.Clear();

	spectator->UpdateSwitches();

	spectator->UpdateHCSr04Seitlich(); // this should not be done always becaue the method is blocking.

	spectator->UpdateHCSr04VorneHinten();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
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
