/** MovementList.cpp
***
*** Diese Klasse verwaltet und erstellt eine List an Bewegungen, welche 
*** zum Beispiel dem Auswichen von Hindernissen dienen kann.
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
#include "MovementList.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MovementListClass::MovementListClass()
{
	this->firstMovement = NULL;
	this->lastAddedMovement = NULL;
	this->currentMovement = NULL;

}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void MovementListClass::AddMovement(int MotorSpeedL, int MotorSpeedR)
{
	if (firstMovement == NULL)
	{
		firstMovement = new ScriptedMovementClass(new MotorDaten(MotorSpeedL, MotorSpeedR));
		lastAddedMovement = firstMovement;
	}
	else
	{
		lastAddedMovement->NextMovement = new ScriptedMovementClass(new MotorDaten(MotorSpeedL, MotorSpeedR));
		lastAddedMovement = lastAddedMovement->NextMovement;
	}
}

void MovementListClass::AddMovement(int MotorSpeedL, int MotorSpeedR, int NumberOfTicks)
{
	for (int i = 0; i < NumberOfTicks; i++)
	{
		AddMovement(MotorSpeedL, MotorSpeedR);
	}
}

ScriptedMovementClass* MovementListClass::GetNextMovement()
{
	if (currentMovement != NULL)
	{
		currentMovement = currentMovement->NextMovement;
	}
	
	return currentMovement;
}

void MovementListClass::GoToStart()
{
	currentMovement = firstMovement;
}
