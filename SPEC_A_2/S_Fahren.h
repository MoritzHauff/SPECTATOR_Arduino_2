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
class S_FahrenClass : public StateClass   // todo: Zeitersparnis mit static / const???  // public: Zugriff auf BasisKlasse ermöglichen.
{
 protected:
	 bool toggleState;

 public:
	 S_FahrenClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
		{ }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();
};

#endif
