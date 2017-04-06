// S_Calibrate.h - Moritz Hauff - 06.04.2017

#ifndef _S_CALIBRATE_h
#define _S_CALIBRATE_h


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
const int S_Calibrate_MPU_Timer = 40000;   // Gibt an nach welcher Zeit [ms] eine MPU-Kalibrierung abgebrochen werden soll.

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State kalibriert das MPU6050.*/
class S_CalibrateClass : public StateClass
{
 protected:
	 void MPUCalibration();

 public:
	 S_CalibrateClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }

	 void Init();
	 void Sense() { }
	 void Think() { }
	 void Act() { }

	 void ShiftTimers(unsigned long ShiftAmount) { }
};

#endif
