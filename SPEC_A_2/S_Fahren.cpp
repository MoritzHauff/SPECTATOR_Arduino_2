/** S_Fahren.cpp
***
*** Beschreibt den StandardFahrmodus.
***
*** Moritz Hauff, 18.03.2017
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
#include "S_Fahren.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_FahrenClass::Init()
{
	toggleState = false;
}

///////////////////////////////////////////////////////////////////////////
///Functions
void S_FahrenClass::Sense()
{
	spectator->UpdateSharp();

	if (toggleState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		spectator->UpdateMLX();
	}

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_FahrenClass::Think()
{
	toggleState = !toggleState;
	/*Serial.print("ToggleState: ");
	Serial.println(toggleState);*/
}

void S_FahrenClass::Act()
{
	spectator->Motoren.SetMotoren(MotorSpeedL, MotorSpeedR);

	//Serial.println("Motorspeed gesetzt");

	spectator->HeartbeatLED.Toggle();
}
