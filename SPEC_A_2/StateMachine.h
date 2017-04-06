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

///////////////////////////////////////////////////////////////////////////
///Konstanten
#define SERIALBUFF_SIZE 32 // make it big enough to hold your longest command

///////////////////////////////////////////////////////////////////////////
///StateMachine-Class
/*Diese Klasse �berwacht die aktuellen Aufgaben des Roboters.
Analysiert au�erdem die seriellen Befehle.*/
class StateMachineClass
{
 protected:
	 SPECTATORClass *spectator;

	 S_TeleOpClass *s_TeleOp;
	 S_DrehenClass *s_Drehen;
	 S_CoffeeBreakClass *s_CoffeeBreak;

	 StateClass *currentState;
	 
	 /*�berwacht den seriellen Eingangsstream und teilt diesen in einzelne Befehle auf,
	 die dann analysiert werden k�nnen. Dies sollte stets einmal pro loop-Durchgang geschehen.*/
	 void handleSerial();
	 /*Analysiert die seriellen Befehle und wechselt bei Bedarf den aktuellen Modus.*/
	 void handleReceivedMessage(char *msg);
	 /*Wechselt in einen anderen Modus und f�hrt die initilaisierunsfunktion des neuen Modus aus.*/
	 void changeState(StateClass *NextState);
	 /*F�hrt einen in die KaffeePause gegangenen State fort. Dabei wird dieser nicht initialisiert
	 und damit die Variablen nicht ver�ndert.*/  // todo: achtung timer!
	 void resumeState(StateClass *State);

	 //Hilfsfunktionen
	 /*Gibt bei '+' 1, '-' -1 und sonst 0 zur�ck.*/
	 int convertCharToVorzeichen(char c);

 public:
	 StateMachineClass(SPECTATORClass *Spectator);
	 ~StateMachineClass();
	 void Init();
	
	 /*F�hrt die im aktuellen Modus ben�tigten Schritte aus. Dies sollte einmal in jedem loop() geschehen.*/
	 void DoAction();
};

#endif
