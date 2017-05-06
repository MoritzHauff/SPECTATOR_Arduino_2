// S_Drehen.h - Moritz Hauff - 19.03.2017

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

const int S_Drehen_MinSpeed = 84;  // Die niedrigste Geschwindigkeit bei denen die Motoren den SPECTATOR noch bewegen.
const int S_Drehen_MaxSpeed = 187;  // Die h�chste Motorgeschwindigkeit beim Drehen.

const int S_Drehen_MotorSteigung = 320;  // Mit welcher Steigung die Motorgeschwindigkeit mit gr��erem MotorAbstand zunimmt.

const int S_Drehen_StuckSteigung = 15;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Beschreibt den Drehe-nach-Himmelsrichtung-Modus.*/
class S_DrehenClass : public StateClass
{
 protected:
	 unsigned long startTime;
	 int counter;
	 int finishedCounter;

	 int MotorSpeedL;
	 int MotorSpeedR;

	 float letzterWinkelAbstand;
	 int stuckCounter;

	 /*F�hrt alle Aktionen w�hrend dem normalen Drehen durch.*/
	 void running();

 public:
	 S_DrehenClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 {
		 ZielRichtung = R_NORDEN;
	 }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount);

	 byte ZielRichtung;
	 
};

#endif
