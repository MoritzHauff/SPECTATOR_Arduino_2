// MovementArray.h - Moritz Hauff - 27.04.2017

#ifndef _MOVEMENTARRAY_h
#define _MOVEMENTARRAY_h

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
/*Stellt Funktionen zur bearbeitung von Movement Arrays bereit.*/
class MovementArrayClass
{
 protected:
	 int aktPos;

 public:
	 /// <summary>
	 /// Setzt die aktuelle Array-Position auf die erste Bewegung und beginnt
	 /// so mit der Liste von Bewegungen von vorne.
	 /// </summary>
	 void GoToStart();

	 /// <summary>
	 /// Gibt die in der Liste nächste Bewegung zurück und wandert mit der
	 /// aktuellen Array-Postion eine Stelle weiter. Gibt null zurück, wenn keine weitere
	 /// Bewegung mehr geplant ist.
	 /// </summary>
	 MotorDaten* GetNextMovement(const int MovementsLinks[], const int MovementsRechts[], const int Rows);
};

#endif

