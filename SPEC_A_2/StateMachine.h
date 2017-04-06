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
#include "S_TeleOp.h"
#include "S_Drehen.h"
#include "S_CoffeeBreak.h"
#include "S_Calibrate.h"
#include "S_GeradeAus.h"

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

	 S_TeleOpClass *s_TeleOp;
	 S_DrehenClass *s_Drehen;
	 S_CoffeeBreakClass *s_CoffeeBreak;
	 S_CalibrateClass *s_Calibrate;
	 S_GeradeAusClass *s_GeradeAus;

	 StateClass *currentState;

	 bool startedNewAction;
	 
	 /*Überwacht den seriellen Eingangsstream und teilt diesen in einzelne Befehle auf,
	 die dann analysiert werden können. Dies sollte stets einmal pro loop-Durchgang geschehen.*/
	 void handleSerial();
	 /*Analysiert die seriellen Befehle und wechselt bei Bedarf den aktuellen Modus.*/
	 void handleReceivedMessage(char *msg);
	 /*Wechselt in einen anderen Modus und führt die initilaisierunsfunktion des neuen Modus aus.*/
	 void changeState(StateClass *NextState);
	 /*Führt einen in die KaffeePause gegangenen State fort. Dabei wird dieser nicht initialisiert
	 und damit die Variablen nicht verändert.*/  // todo: achtung timer!
	 void resumeState(StateClass *State, unsigned long TimerShiftAmount);

	 //Hilfsfunktionen
	 /*Gibt bei '+' 1, '-' -1 und sonst 0 zurück.*/
	 int convertCharToVorzeichen(char c);

 public:
	 StateMachineClass(SPECTATORClass *Spectator);
	 ~StateMachineClass();
	 void Init();
	
	 /*Führt die im aktuellen Modus benötigten Schritte aus. Dies sollte einmal in jedem loop() geschehen.*/
	 void DoAction();
	 /*Ermöglicht dem Overwachter ebenfalls Einfluss auf den aktuellen State zu nehmen.
	 Empfängt die gleichen Befehle wie über die serielle Schnittstelle nur eben nicht seriell.*/
	 void SendDirectCommand(char *msg);

	 /*Gibt an ob seit dem letzten Aufruf dieser Funktion eine neue Aktion gestartet wurde.
	 Dies wird vom Overwatcher dazu verwendet den gesamtFortschritt zu überwachen.*/
	 bool StartedNewAction();
};

#endif
