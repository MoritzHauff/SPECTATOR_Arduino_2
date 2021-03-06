// S_SchwaresFeld.h - Moritz Hauff - 23.04.2017

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

#ifndef _S_SCHWARZESFELD_h
#define _S_SCHWARZESFELD_h

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
/*Dieser State dient dem Verlassen eines schwarzen Feldes.*/
class S_SchwarzesFeldClass : public StateClass
{
///////////////////////////////////////////////////////////////////////////
 protected: ///Konstanten
	 const static int S_SchwarzesFeld_MaxTimer = 1500;
	 const static int S_SchwarzesFeld_USHinten = 7;

 protected:
	 unsigned long startTime;
	 
	 int speedL;
	 int speedR;
	 int encoderL;
	 int encoderR;

	 int stoppWahrscheinlichkeit;


 public:
	S_SchwarzesFeldClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	{ }

	void Init();
	void Sense();
	void Think();
	void Act();

	void ShiftTimers(unsigned long ShiftAmount);

	int ExpectedNumberOfEncoderTicks;
};

#endif

