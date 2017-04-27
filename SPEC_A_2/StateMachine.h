// StateMachine.h - Moritz Hauff - 17.03.2017

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
#include "S_Sense.h"
#include "S_Idle.h"
#include "S_SchwarzesFeld.h"
#include "S_Rampe.h"
#include "S_ScriptedMovement.h"

///////////////////////////////////////////////////////////////////////////
///Konstanten
#define SERIALBUFF_SIZE 32 // make it big enough to hold your longest command

///////////////////////////////////////////////////////////////////////////
///StateMachine-Class
/// <summary>
/// Diese Klasse überwacht die aktuellen Aufgaben des Roboters. <para/>
/// Analysiert außerdem die seriellen Befehle.
/// </summary>
class StateMachineClass
{
 protected:
	 SPECTATORClass *spectator;

	 S_IdleClass *s_Idle /* = S_IdleClass(spectator, "Idle")*/;
	 S_TeleOpClass *s_TeleOp;
	 S_DrehenClass *s_Drehen;
	 S_CoffeeBreakClass *s_CoffeeBreak;
	 S_CalibrateClass *s_Calibrate;
	 S_GeradeAusClass *s_GeradeAus;
	 S_SenseClass *s_Sense;
	 S_SchwarzesFeldClass *s_SchwarzesFeld;
	 S_RampeClass *s_Rampe;
	 S_ScriptedMovementClass *s_ScriptedMovement;

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
	 /*Kontrolliert ob der aktuelle State fertig ist und wechselt ihn gegebenenfalls.*/
	 void checkStates();

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

	 /*Funktionszeiegr zur Kommunikation mit dem Overwatcher.*/
	 void(*OverwatcherMsg)(String Msg);

	 String GetCurrentState() { return currentState->GetName(); }
};

#endif
