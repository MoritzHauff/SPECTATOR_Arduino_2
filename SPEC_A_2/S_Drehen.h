// S_Drehen.h - Moritz Hauff - 19.03.2017

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

#ifndef _S_DREHEN_h
#define _S_DREHEN_h

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
const int S_Drehen_Timer = 3000;   // Nach wie vielen ms das Drehen mit einer Warnung beednet werden soll.

const int S_Drehen_MinSpeed = 82;  // Die niedrigste Geschwindigkeit bei denen die Motoren den SPECTATOR noch bewegen.
const int S_Drehen_MaxSpeed = 180;  // Die höchste Motorgeschwindigkeit beim Drehen.

const int S_Drehen_MotorSteigung = 320;  // Mit welcher Steigung die Motorgeschwindigkeit mit größerem MotorAbstand zunimmt.

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Beschreibt den Drehe-nach-Himmelsrichtung-Modus.*/
class S_DrehenClass : public StateClass
{
 protected:
	 unsigned long startTime;
	 int counter;

	 int MotorSpeedL;
	 int MotorSpeedR;

	 /*Führt alle Aktionen während dem normalen Drehen durch.*/
	 void running();

 public:
	 S_DrehenClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount);

	 byte ZielRichtung;
	 
};

#endif
