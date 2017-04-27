/** MovementList.cpp
***
*** Diese Klasse verwaltet und erstellt eine List an Bewegungen, welche 
*** zum Beispiel dem Auswichen von Hindernissen dienen kann.
***
*** Moritz Hauff, 27.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "MovementList.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MovementListClass::MovementListClass()
{
	this->firstMovement = NULL;
	this->lastAddedMovement = NULL;
	this->currentMovement = NULL;

}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void MovementListClass::AddMovement(int MotorSpeedL, int MotorSpeedR)
{
	if (firstMovement == NULL)
	{
		firstMovement = new ScriptedMovementClass(new MotorDaten(MotorSpeedL, MotorSpeedR));
		lastAddedMovement = firstMovement;
	}
	else
	{
		lastAddedMovement->NextMovement = new ScriptedMovementClass(new MotorDaten(MotorSpeedL, MotorSpeedR));
		lastAddedMovement = lastAddedMovement->NextMovement;
	}
}

void MovementListClass::AddMovement(int MotorSpeedL, int MotorSpeedR, int NumberOfTicks)
{
	for (int i = 0; i < NumberOfTicks; i++)
	{
		AddMovement(MotorSpeedL, MotorSpeedR);
	}
}

ScriptedMovementClass* MovementListClass::GetNextMovement()
{
	if (currentMovement != NULL)
	{
		currentMovement = currentMovement->NextMovement;
	}
	
	return currentMovement;
}

void MovementListClass::GoToStart()
{
	currentMovement = firstMovement;
}
