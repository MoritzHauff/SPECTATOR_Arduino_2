// StateMachine.h - Moritz Hauff - 17.03.2017

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

#ifndef _STATEMACHINE_h
#define _STATEMACHINE_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"

#include "SPECTATORClass.h"
#include "S_Fahren.h"

///////////////////////////////////////////////////////////////////////////
///Konstanten
#define SERIALBUFF_SIZE 32 // make it big enough to hold your longest command

///////////////////////////////////////////////////////////////////////////
///StateMachine-Class
/*Diese Klasse überwacht die aktuellen Aufgaben des Roboters.
Analysiert außerdem die seriellen Befehle.*/
class StateMachineClass
{
 protected:
	 SPECTATORClass *spectator;

	 S_FahrenClass *s_Fahren;

	 StateClass *currentState;
	 
	 /*Analysiert die seriellen Befehle und wechselt bei Bedarf den aktuellen Modus.*/
	 void handleSerial();
	 /*Wechslet in einen anderen Modus und führt die initilaisierunsfunkiton des neuen modus aus.*/
	 void changeState(StateClass *NextState);

	 //Hilfsfunktionen
	 int convertCharToVorzeichen(char c);
	 void handleReceivedMessage(char *msg);

 public:
	 StateMachineClass(SPECTATORClass *Spectator);
	 ~StateMachineClass();
	
	 /*Führt die im aktuellen Modus benötigten Schritte aus. Dies sollte einmal in jedem loop() geschehen.*/
	 void DoAction();
};

#endif
