// Overwatcher.h - Moritz Hauff - 06.04.2017

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

#ifndef _OVERWATCHER_h
#define _OVERWATCHER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SPECTATORClass.h"
#include "StateMachine.h"

///////////////////////////////////////////////////////////////////////////
///Overwatcher-Class
/*Diese Klasse überwacht den globalen Zustand und einzelne Aktionen. Dazu 
sammelt sie die Fehlermeldungen der einzelnen Komponenten und greift gegebenfalls 
mit Recover-Funktionen in das Geschehen ein.*/
class OverwatcherClass
{
 protected:
	 StateMachineClass *stateMachine;

	 int actions;   // beeinhalt die seit dem ArduinoStart durchgeführte Aktionen.

 public:
	 void Init(StateMachineClass *StateMachine);

	 /*Führt eine Statuskontrolle durch. Dies sollte in jedem (2.) loop Durchlauf geschehen.*/
	 void Control();

	 int GetActions() { return actions; }
};

extern OverwatcherClass OW;

#endif
