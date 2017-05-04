/** MovementArray.cpp
***
*** Diese Klasse verwaltet und erstellt eine List an Bewegungen, welche
*** zum Beispiel dem Auswichen von Hindernissen dienen kann.
***
*** Moritz Hauff, 27.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "MovementArray.h"

///////////////////////////////////////////////////////////////////////////
///Funktionen
void MovementArrayClass::GoToStart()
{
	aktPos = 0;
}

MotorDaten* MovementArrayClass::GetNextMovement(const int MovementsLinks[], const int MovementsRechts[], const int Rows)
{
	if (aktPos < Rows-1 && aktPos >= -1)
	{
		aktPos++;
		return new MotorDaten(MovementsLinks[aktPos], MovementsRechts[aktPos]);
	}

	return NULL;
}
