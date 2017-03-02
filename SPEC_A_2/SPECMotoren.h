// SPECMotoren.h - Moritz Hauff - 02.03.2017

#ifndef _SPECMOTOREN_h
#define _SPECMOTOREN_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_MotorShield.h>

///////////////////////////////////////////////////////////////////////////
///Constants
const int PWR_LEDRichtung = FORWARD;
const int PWR_MotorenRichtung = BACKWARD;

///////////////////////////////////////////////////////////////////////////
///SPECMotorenClass
/*Steuert das Adafruit Motorshield.*/
class SPECMotorenClass
{
 protected:
	 Adafruit_MotorShield AFMS = Adafruit_MotorShield();

	 Adafruit_DCMotor *MotorL;
	 Adafruit_DCMotor *MotorR;
	 Adafruit_DCMotor *RescueMotoren;
	 Adafruit_DCMotor *UnterflurBeleuchtung;

	 bool SetMotorL(int Speed);
	 bool SetMotorR(int Speed);

 public:
	SPECMotorenClass(uint8_t MotorPortLinks, uint8_t MotorPortRechts, uint8_t RescueMotorPort, uint8_t UnterflurPort);
	void Init();

	bool SetMotoren(int SpeedL, int SpeedR);
	void Kontrolllauf();

	/*Aktiviert die Unterflurbeleuchtung.*/
	void TurnLEDOn();
	/*Deaktivieert die Unterflurbeleuchtung.*/
	void TurnLEDOff();

	/*Aktiviert die Rescue-Kit Motoren.*/
	void TurnRescueOn();
	/*Deaktiviert die Rescue-Kit Motoren.*/
	void TurnRescueOff();
};

#endif
