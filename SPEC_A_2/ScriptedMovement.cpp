/** ScriptedMovement.cpp
***
*** Diese Klasse speichert einzelne Motordaten und die jeweils darauffolgenden.
*** Sie dinet damit dem einspeichern von Recover-Verhalten.
***
*** Moritz Hauff, 27.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "ScriptedMovement.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
ScriptedMovementClass::ScriptedMovementClass(MotorDaten MotorDaten)
{
	this->motorDaten = MotorDaten;
	this->NextMovement = NULL;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
int ScriptedMovementClass::GetMotorSpeedL()
{
	return this->motorDaten.MotorSpeedL;
}

int ScriptedMovementClass::GetMotorSpeedR()
{
	return this->motorDaten.MotorSpeedR;
}
