// S_Idle.h - Moritz Hauff - 21.04.2017

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

#ifndef _S_IDLE_h
#define _S_IDLE_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "State.h"

///////////////////////////////////////////////////////////////////////////
///State-Class
const int S_Idle_NumberOfTicks = 300;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State aktualisiert nur das MPU damit es zu keinem BufferOverflow kommt.*/
class S_IdleClass : public StateClass
{
protected:
	int counter;

public:
	S_IdleClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	{ }

	void Init();
	void Sense();
	void Think();
	void Act();

	void ShiftTimers(unsigned long ShiftAmount);
};

#endif
