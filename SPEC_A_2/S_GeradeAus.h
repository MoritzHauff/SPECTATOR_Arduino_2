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
const byte S_GeradeAus_NormalSpeed = 210;	// Die normale Bewegungsgeschwindigkeit.  // 240 zu schnell f�r rampen abw�rts.
const int S_GeradeAus_FeldTraversTimer = 1275;	//1000	// Wie lange ein Feld normalerweise ben�tigt zu befahren.
const int S_GeradeAus_MaxTimer = 1675;	//1300	// Wie lange maximal geradeaus gefahren werden kann, bevor der Modus erneut gewechselt werden muss.
const int S_GeradeAus_MaxStoppWahrscheinlichkeit = 100;
const int S_GeradeAus_WinkelRatio = 300;		// Wie stark die Winkelabweichung in die Fahrgeschwindigkeit wirkt.  //75


//const int S_GeradeAus_WandEntfernungsKorrektur = 70;

const int S_GeradeAus_MinUltraschallEntfernung = 6;   // Ab welcher Entfernung die Ultraschallsensoren eine Weiterfahrt unterbinden sollen.


const int S_Drehen_SharpMin = 150;  // Sobald die W�nder weiter weg sind, also unter diesem Wert liegen, nicht mehr zur Feldkorrektur verwenden.

///////////////////////////////////////////////////////////////////////////
///State-Class
/*Dieser State �berwacht das GeradeAusfahren. (Sowohl vorw�rts als auch r�ckw�rts).*/
class S_GeradeAusClass : public StateClass
{
 protected:
	 const int S_GeradeAus_WandEntfernungen[6] = { 80, 395, 695, 995, 1295, 1595 };  // Die "echten" Wandentfenrungn zur ermittlung der richtigen Kategorie im Stand.
	 const int S_GeradeAus_WandEntfernungenVor[6] = { 130, 460, 760, 1060, 1360, 1660 };  // Die Wandkategorien f�r die Vorw�rtsfahrt.
	 const int S_GeradeAus_WandEntfernungenRueck[6] = { 30, 340, 640, 940, 1240, 1540 };  // Die Wandkategorien f�r die R�ckw�rtsfahrt.

	 unsigned long startTime;

	 bool toggleState;
	 int speedL, adaptedSpeedL;
	 int speedR, adaptedSpeedR;
	 byte startRichtung;

	 int startDistanceLaserV, startDistanceUSV, startDistanceUSH;
	 byte startWandKategorie, zielWandKategorie;

	 int stoppWahrscheinlichkeit;  // wie stark die momentanen Informationen daf�r sprechen anzuhalten.
	 float winkelKorrektur;

	 int encoderL, encoderR;

	 int straightCounter;  // Gibt an wie viele Ticks ganz gerade aus gefahren werden soll, z.B. beim Ausgleich der Wandentfernung.
	 int turnCounter; // Gibt an wie viele Ticks mit einer bestimmten geschwindigkiet gedereht werdn soll bevor der straight COunter zum Einsazt kommt.

	 byte ermittleStartWandKategorie();
	 byte errechneZielWandKategorie(byte StartWandKategorie, int Direction);

	 byte entfernungZuWandKategorie(int aktDistance, int Direction);
	 bool abweichungZuGros(int Value1, int Value2, int MaxAbweichung);

	 void kontrolliereFortschritt();
	 float verwerteSharp(int Vorne, int Hinten);

	 int aktZielEntfernung;
	 bool mitLaser;

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

	 int GetEncoderL() { return encoderL; }
	 int GetEncoderR() { return encoderR; }
};

#endif
