// S_Rampe.h - Moritz Hauff - 23.04.2017

#ifndef _S_RAMPE_h
#define _S_RAMPE_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "State.h"

const static int S_Rampe_NormalSpeedUp = 125; // 140 kippt bei hindernissen  // 120 sehr langsam kippt aber nicht immer.
const static int S_Rampe_NormalSpeedDownBegin = 180;
const static int S_Rampe_NormalSpeedDownEnd = 125;
const static int S_Rampe_USVorne = 4;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State dient dem Befahren der Rampe.*/
class S_RampeClass : public StateClass
{ 
 protected:
	 int stoppWahrscheinlichkeit;   // todo move to StateClass
	 int speedL;  // todo move to StateClass
	 int speedR;
	 byte startRichtung;
	 float winkelKorrektur;

	 float verwerteSharp(int Vorne, int Hinten);

 public:
	 S_RampeClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount);

	 // 'd' zum runterfahren, 'u' zum hochfahren.
	 char Richtung;
	 int NormalSpeed;
};

#endif
