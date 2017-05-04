// MovementArray.h - Moritz Hauff - 27.04.2017

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

#ifndef _MOVEMENTARRAY_h
#define _MOVEMENTARRAY_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///MoterDaten-Struct
struct MotorDaten
{
	int MotorSpeedL;
	int MotorSpeedR;

	MotorDaten(int MotorSpeedL = 0, int MotorSpeedR = 0)
	{
		this->MotorSpeedL = MotorSpeedL;
		this->MotorSpeedR = MotorSpeedR;
	}
};

///////////////////////////////////////////////////////////////////////////
///ScriptedMovement-Class
/*Stellt Funktionen zur bearbeitung von Movement Arrays bereit.*/
class MovementArrayClass
{
 protected:
	 int aktPos;

 public:
	 /// <summary>
	 /// Setzt die aktuelle Array-Position auf die erste Bewegung und beginnt
	 /// so mit der Liste von Bewegungen von vorne.
	 /// </summary>
	 void GoToStart();

	 /// <summary>
	 /// Gibt die in der Liste nächste Bewegung zurück und wandert mit der
	 /// aktuellen Array-Postion eine Stelle weiter. Gibt null zurück, wenn keine weitere
	 /// Bewegung mehr geplant ist.
	 /// </summary>
	 MotorDaten* GetNextMovement(const int MovementsLinks[], const int MovementsRechts[], const int Rows);
};

#endif

