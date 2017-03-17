// S_Fahren.h - Moritz Hauff - 17.03.2017

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

#ifndef _S_FAHREN_h
#define _S_FAHREN_h

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
/*Beschreibt den StandardFahrmodus.*/
class S_FahrenClass : public StateClass   // todo: Zeitersparnis mit static / const???  // public: Zugriff auf BasisKlasse ermöglichen.
{
 protected:
	 bool toggleState;

 public:
	 S_FahrenClass(SPECTATORClass *Spectator) : StateClass(Spectator)
		{ }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();
};

#endif
