// S_Fahren.h - Moritz Hauff - 17.03.2017

#ifndef _S_FAHREN_h
#define _S_FAHREN_h

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
/*Beschreibt den StandardFahrmodus.*/
class S_TeleOpClass : public StateClass   // todo: Zeitersparnis mit static / const???  // public: Zugriff auf BasisKlasse ermöglichen.
{
 protected:
	 bool toggleState;

 public:
	 S_TeleOpClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
		{ }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 int MotorSpeedL;
	 int MotorSpeedR;

};

#endif
