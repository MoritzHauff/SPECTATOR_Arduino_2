// MovementList.h - Moritz Hauff - 27.04.2017

#ifndef _MOVEMENTLIST_h
#define _MOVEMENTLIST_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ScriptedMovement.h"

///////////////////////////////////////////////////////////////////////////
///ScriptedMovement-Class
/// <summary>
/// Diese Klasse verwaltet und erstellt eine List an Bewegungen, welche 
/// zum Beispiel dem Auswichen von Hindernissen dienen kann.
/// </summary>
class MovementListClass
{
 protected:
	 ScriptedMovementClass* firstMovement;
	 ScriptedMovementClass* lastAddedMovement;
	 ScriptedMovementClass* currentMovement;

 public:
	 MovementListClass();

	 /// <summary>
	 /// Fügt der Liste an Bewegungen eine einzelne hinzu und aktualisiert die Referenzen.
	 /// </summary>
	 void AddMovement(int MotorSpeedL, int MotorSpeedR);
	 /// <summary>
	 /// Fügt der Liste an Bewegungen eine Bewegung numberOfTicks-mal hinzu und aktualisiert die Referenzen.
	 /// </summary>
	 void AddMovement(int MotorSpeedL, int MotorSpeedR, int NumberOfTicks);

	 /// <summary>
	 /// Gibt die in der Liste nächste Bewegung zurück und wandert mit dem 
	 /// HEAD-Pointer eine Stelle weiter. Gibt null zurück, wenn keine weitere 
	 /// Bewegung mehr geplant ist.
	 /// </summary>
	 ScriptedMovementClass* GetNextMovement();

	 /// <summary>
	 /// Setzt den HEAD-Pointer auf die erste Bewegung und beginnt 
	 /// so mit der Liste von Bewegungen von vorne.
	 /// </summary>
	 void GoToStart();
};

#endif
