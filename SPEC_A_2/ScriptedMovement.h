// ScriptedMovement.h - Moritz Hauff - 27.04.2017

#ifndef _SCRIPTEDMOVEMENT_h
#define _SCRIPTEDMOVEMENT_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///MoterDaten-Struct
struct MotorDaten
{
	int MotorSpeedL;
	int MotorSpeedR;

	MotorDaten(int MotorSpeedL = 0, int MotorSpeedR = 0)
	{
		this->MotorSpeedL = MotorSpeedL;
		this->MotorSpeedR = MotorSpeedR;
	}
};

///////////////////////////////////////////////////////////////////////////
///ScriptedMovement-Class
/// <summary>
/// Diese Klasse speichert einzelne Motordaten und die jeweils darauffolgenden.
/// </summary>
class ScriptedMovementClass
{
 protected:
	 MotorDaten motorDaten;

 public:
	 ScriptedMovementClass(MotorDaten MotorDaten);
	 
	 ScriptedMovementClass* NextMovement;

	 int GetMotorSpeedL();
	 int GetMotorSpeedR();
	 
};

#endif
