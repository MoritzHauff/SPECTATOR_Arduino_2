// S_GeradeAus.h - Moritz Hauff - 06.04.2017

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

#ifndef _S_GERADEAUS_h
#define _S_GERADEAUS_h

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
const byte S_GeradeAus_NormalSpeed = 240;	// Die normale Bewegungsgeschwindigkeit.
const int S_GeradeAus_FeldTraversTimer = 1000;		// Wie lange ein Feld normalerweise benötigt zu befahren.
const int S_GeradeAus_MaxTimer = 1300;		// Wie lange maximal geradeaus gefahren werden kann, bevor der Modus erneut gewechselt werden muss.
const int S_GeradeAus_MaxStoppWahrscheinlichkeit = 100;


///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State überwacht das GeradeAusfahren. (Sowohl vorwärts als auch rückwärts).*/
class S_GeradeAusClass : public StateClass
{
 protected:
	 unsigned long startTime;

	 bool toggleState;
	 int speedL;
	 int speedR;

	 int stoppWahrscheinlichkeit;  // wie stark die momentanen Informationen dafür sprechen anzuhalten.
	 float winkelKorrektur;

 public:
	 S_GeradeAusClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount);

	 int Direction;
};

#endif
