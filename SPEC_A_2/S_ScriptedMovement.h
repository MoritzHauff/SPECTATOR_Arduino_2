// S_ScriptedMovement.h - Moritz Hauff - 27.04.2017

#ifndef _S_SCRIPTEDMOVEMENT_h
#define _S_SCRIPTEDMOVEMENT_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "State.h"
#include "MovementArray.h"
#include "MovementArrays.h"

///////////////////////////////////////////////////////////////////////////
///enum Ausweichmanöver
enum AusweichBewegung
{
	BumperLinks,
	BumperRechts
};

///////////////////////////////////////////////////////////////////////////
///State-Class
/// <summary>
/// Dieser State dient dem Abfahren eines vorgelegten Scripts (MovementArray) <para/>
/// welches die genauen Motorgeschwindigkeiten für jeden Tick beinhaltet. <para/>
/// Dabei laufen nur die grundlegensten Zusammenstoßverhinderungsmaßnahmen. <para/>
/// </summary>
class S_ScriptedMovementClass : public StateClass
{
///////////////////////////////////////////////////////////////////////////
 protected: ///Konstanten 
	 static const int S_ScriptedMovement_MaxTime = 2000;

 protected:
	 MovementArrayClass movementArrayHandler = MovementArrayClass();

	 unsigned long startTime;

 public:
	 S_ScriptedMovementClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount);

	 AusweichBewegung AusweichBewegung;
};

#endif
