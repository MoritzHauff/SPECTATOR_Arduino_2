// Overwatcher.h - Moritz Hauff - 06.04.2017

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
/// Konstanten
const static float C_Overwatcher_RampenWinkel = 0.2;

///////////////////////////////////////////////////////////////////////////
///Overwatcher-Class
/*Diese Klasse �berwacht den globalen Zustand und einzelne Aktionen. Dazu 
sammelt sie die Fehlermeldungen der einzelnen Komponenten und greift gegebenfalls 
mit Recover-Funktionen in das Geschehen ein.*/
class OverwatcherClass  
{
///////////////////////////////////////////////////////////////////////////
 protected:   /// Konstanten
	 const static int C_SchwarzesFeld = 870;

 //protected:
 public: 
	 StateMachineClass *stateMachine;

	 int actions;   // beeinhalt die seit dem ArduinoStart durchgef�hrte Aktionen.

	 static void ErrorHandler(String Msg);   // you need static for the function pointer

	 int rampenCounter;
	 int drehFehlerCounter;
	 int drehFehlerResetCounter;

	 int checkpointCounter;

 public:
	 void Init(StateMachineClass *StateMachine);

	 /*F�hrt eine Statuskontrolle durch. Dies sollte in jedem (2.) loop Durchlauf geschehen.*/
	 void Control();

	 int GetActions() { return actions; }
};

extern OverwatcherClass OW;

#endif
