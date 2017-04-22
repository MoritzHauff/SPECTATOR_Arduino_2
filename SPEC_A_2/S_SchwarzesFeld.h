// S_SchwaresFeld.h - Moritz Hauff - 23.04.2017

#ifndef _S_SCHWARZESFELD_h
#define _S_SCHWARZESFELD_h

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
/*Dieser State dient dem Verlassen eines schwarzen Feldes.*/
class S_SchwarzesFeldClass : public StateClass
{
///////////////////////////////////////////////////////////////////////////
 protected: ///Konstanten
	 const static int S_SchwarzesFeld_MaxTimer = 400;
	 const static int S_SchwarzesFeld_USHinten = 7;

 protected:
	 unsigned long startTime;
	 
	 int speedL;
	 int speedR;
	 int encoderL;
	 int encoderR;

	 int stoppWahrscheinlichkeit;


 public:
	S_SchwarzesFeldClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	{ }

	void Init();
	void Sense();
	void Think();
	void Act();

	void ShiftTimers(unsigned long ShiftAmount);

	int ExpectedNumberOfEncoderTicks;
};

#endif

