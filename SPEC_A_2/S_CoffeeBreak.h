// S_CoffeeBreak.h - Moritz Hauff - 06.04.2017

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

#ifndef _S_COFFEEBREAK_h
#define _S_COFFEEBREAK_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "State.h"

///////////////////////////////////////////////////////////////////////////
///Konstanten
const int BlinkDelay = 400;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Diese Klasse ermöglicht dem seriellen Operator die aktuelle Aktion zu 
unterbrechen und sie nach einem weiteren Befehl wieder aufzunehmen.
Dies soll dem Debugging dienen.*/
class S_CoffeeBreakClass : public StateClass
{
 protected:
	 unsigned long coffeBreakStart;
	 unsigned long lastToggle;

 public:
	 S_CoffeeBreakClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }
	 
	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount) { /* in the CoffeeBreak are no timers. */ }

	 /*Gibt die verstrichenen Zeit [ms] seit der Aktivierung der KaffePause zurück,
	 um beim Wiederaufnehmen der alten Aktion die Timer anpassen zu können.*/
	 unsigned long GetTimerShiftAmount();

	 StateClass *LastState;
};

#endif
