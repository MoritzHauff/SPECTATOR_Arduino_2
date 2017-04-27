// MovementList.h - Moritz Hauff - 27.04.2017

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

#ifndef _MOVEMENTLIST_h
#define _MOVEMENTLIST_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ScriptedMovement.h"

///////////////////////////////////////////////////////////////////////////
///ScriptedMovement-Class
/// <summary>
/// Diese Klasse verwaltet und erstellt eine List an Bewegungen, welche 
/// zum Beispiel dem Auswichen von Hindernissen dienen kann.
/// </summary>
class MovementListClass
{
 protected:
	 ScriptedMovementClass* firstMovement;
	 ScriptedMovementClass* lastAddedMovement;
	 ScriptedMovementClass* currentMovement;

 public:
	 MovementListClass();

	 /// <summary>
	 /// Fügt der Liste an Bewegungen eine einzelne hinzu und aktualisiert die Referenzen.
	 /// </summary>
	 void AddMovement(int MotorSpeedL, int MotorSpeedR);
	 /// <summary>
	 /// Fügt der Liste an Bewegungen eine Bewegung numberOfTicks-mal hinzu und aktualisiert die Referenzen.
	 /// </summary>
	 void AddMovement(int MotorSpeedL, int MotorSpeedR, int NumberOfTicks);

	 /// <summary>
	 /// Gibt die in der Liste nächste Bewegung zurück und wandert mit dem 
	 /// HEAD-Pointer eine Stelle weiter. Gibt null zurück, wenn keine weitere 
	 /// Bewegung mehr geplant ist.
	 /// </summary>
	 ScriptedMovementClass* GetNextMovement();

	 /// <summary>
	 /// Setzt den HEAD-Pointer auf die erste Bewegung und beginnt 
	 /// so mit der Liste von Bewegungen von vorne.
	 /// </summary>
	 void GoToStart();
};

#endif
