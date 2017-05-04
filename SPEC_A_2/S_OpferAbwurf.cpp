/** S_OpferAbwurf.cpp
***
*** Dieser State überwacht den Abwurf von RescueKits.
***
*** Moritz Hauff, 04.05.2017
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
