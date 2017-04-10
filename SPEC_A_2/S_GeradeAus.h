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
#include "MedianFilter.h"

///////////////////////////////////////////////////////////////////////////
///Konstanten
const byte S_GeradeAus_NormalSpeed = 240;	// Die normale Bewegungsgeschwindigkeit.
const int S_GeradeAus_FeldTraversTimer = 1100;	//1000	// Wie lange ein Feld normalerweise benötigt zu befahren.
const int S_GeradeAus_MaxTimer = 1500;	//1300	// Wie lange maximal geradeaus gefahren werden kann, bevor der Modus erneut gewechselt werden muss.
const int S_GeradeAus_MaxStoppWahrscheinlichkeit = 100;
const int S_GeradeAus_WinkelRatio = 300;		// Wie stark die Winkelabweichung in die Fahrgeschwindigkeit wirkt.  //75

const int S_GeradeAus_WandEntfernungen[] = { 90, 410, 710, 1010 };
const int S_GeradeAus_WandEntfernungsKorrektur = 70;  // todo vlt einfach mit zwei arraylisten einmal für voräwrts einmal für rückwärts.

const int S_GeradeAus_MinUltraschallEntfernung = 6;   // Ab welcher Entfernung die Ultraschallsensoren eine Weiterfahrt unterbinden sollen.

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State überwacht das GeradeAusfahren. (Sowohl vorwärts als auch rückwärts).*/
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

	 int stoppWahrscheinlichkeit;  // wie stark die momentanen Informationen dafür sprechen anzuhalten.
	 float winkelKorrektur;

	 int capSpeed(int Value, int Upper, int Lower);
	 int encoderL, encoderR;

	 byte ermittleStartWandKategorie();
	 byte errechneZielWandKategorie(byte StartWandKategorie, int Direction);

	 byte entfernungZuWandKategorie(int aktDistance, int Direction);
	 bool abweichungZuGros(int Value1, int Value2, int MaxAbweichung);

	 void kontrolliereFortschritt();

	 MedianFilterClass laserFilter;

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
