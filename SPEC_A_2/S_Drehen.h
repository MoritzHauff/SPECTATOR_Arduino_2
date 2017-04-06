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
