// S_Idle.h - Moritz Hauff - 21.04.2017

#ifndef _S_IDLE_h
#define _S_IDLE_h

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
const int S_Idle_NumberOfTicks = 300;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State aktualisiert nur das MPU damit es zu keinem BufferOverflow kommt.*/
class S_IdleClass : public StateClass
{
protected:
	int counter;

public:
	S_IdleClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	{ }

	void Init();
	void Sense();
	void Think();
	void Act();

	void ShiftTimers(unsigned long ShiftAmount);
};

#endif
