// State.h - Moritz Hauff - 17.03.2017

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

#ifndef _STATE_h
#define _STATE_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SPECTATORClass.h"

///////////////////////////////////////////////////////////////////////////
///State-Class
class StateClass  // abstrakte Klasse mit virtuellen Funktionen
{
private:
	 String name;
 protected:
	 SPECTATORClass *spectator;

 public:
	 StateClass(SPECTATORClass *Spectator, const char Name[]);
	 // todo: virtueller Destruktor?

	 virtual void Sense() = 0;  // noch keine Methodenimplemtierung -> siehe vererbte Klassen
	 virtual void Think() = 0;
	 virtual void Act() = 0;
	 /*Wird immer aufgerufen wenn in den Modus gewechselt wird.*/
	 virtual void Init() = 0;

	 String GetName();
};

#endif
