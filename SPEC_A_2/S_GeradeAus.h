// S_GeradeAus.h - Moritz Hauff - 06.04.2017

#ifndef _S_GERADEAUS_h
#define _S_GERADEAUS_h

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
const byte S_GeradeAus_NormalSpeed = 240;	// Die normale Bewegungsgeschwindigkeit.
const int S_GeradeAus_FeldTraversTimer = 2800;	//1000	// Wie lange ein Feld normalerweise ben�tigt zu befahren.
const int S_GeradeAus_MaxTimer = 3000;	//1300	// Wie lange maximal geradeaus gefahren werden kann, bevor der Modus erneut gewechselt werden muss.
const int S_GeradeAus_MaxStoppWahrscheinlichkeit = 100;
const int S_GeradeAus_WinkelRatio = 300;		// Wie stark die Winkelabweichung in die Fahrgeschwindigkeit wirkt.  //75

const int S_GeradeAus_WandErntfernungen[] = { 100, 400, 700 };
const int S_GeradeAus_WandEntfernungsKorrektur = 60;

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State �berwacht das GeradeAusfahren. (Sowohl vorw�rts als auch r�ckw�rts).*/
class S_GeradeAusClass : public StateClass
{
 protected:
	 unsigned long startTime;

	 bool toggleState;
	 int speedL, adaptedSpeedL;
	 int speedR, adaptedSpeedR;
	 byte startRichtung;

	 int startDistanceLaserV, startDistanceUSV, startDistanceUSH;
	 byte startWandKategorie, zielWandKategorie;

	 int stoppWahrscheinlichkeit;  // wie stark die momentanen Informationen daf�r sprechen anzuhalten.
	 float winkelKorrektur;

	 int capSpeed(int Value, int Upper, int Lower);
	 int encoderL, encoderR;

	 byte ermittleStartWandKategorie(int aktDistance);

 public:
	 S_GeradeAusClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	 { }

	 void Init();
	 void Sense();
	 void Think();
	 void Act();

	 void ShiftTimers(unsigned long ShiftAmount);

	 int Direction;
};

#endif
