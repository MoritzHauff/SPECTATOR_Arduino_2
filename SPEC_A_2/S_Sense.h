// S_Sense.h - Moritz Hauff - 21.04.2017

#ifndef _S_SENSE_h
#define _S_SENSE_h

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
const int S_Sense_NumberOfTicks = 7;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State erfasst nach jeder Aktion das aktuelle Feld.*/
class S_SenseClass : public StateClass
{
protected:
	int counter;

public:
	S_SenseClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	{ }

	void Init();
	void Sense();
	void Think();
	void Act();

	void ShiftTimers(unsigned long ShiftAmount);
};

#endif
