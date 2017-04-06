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
#include <Stepper.h>
#include "WheelEncoder.h"

///////////////////////////////////////////////////////////////////////////
///Constants
const int PWR_LEDRichtung = FORWARD;

// change this to the number of steps on your motor
#define STEPS 513

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

	 Stepper *stepperL;
	 Stepper *stepperR;

	 bool SetMotorL(int Speed);
	 bool SetMotorR(int Speed);

 public:
	SPECMotorenClass(uint8_t MotorPortLinks, uint8_t MotorPortRechts, uint8_t RescueMotorPort, uint8_t UnterflurPort, 
		uint8_t StepperLPin1, uint8_t StepperLPin2, uint8_t StepperLPin3, uint8_t StepperLPin4,
		uint8_t StepperRPin1, uint8_t StepperRPin2, uint8_t StepperRPin3, uint8_t StepperRPin4);
	void Init();

	WheelEncoderClass encoderLinks;
	WheelEncoderClass encoderRechts;

	bool SetMotoren(int SpeedL, int SpeedR);
	void Kontrolllauf();

	/*Aktiviert die Unterflurbeleuchtung.*/
	void TurnLEDOn();
	/*Deaktivieert die Unterflurbeleuchtung.*/
	void TurnLEDOff();

	/*Wirft links ein Kit ab. Diese Funktion blockt.*/
	void AbwurfLinks();
	/*Dreht den linken RescueMotor um die Anzahl an Umdrehungen.*/
	void StepL(float Revolutions);
	/*Wirft rechts ein Kit ab. Diese Funktion blockt.*/
	void AbwurfRechts();
	/*Dreht den rechten RescueMotor um die Anzahl an Umdrehungen.*/
	void StepR(float Revolutions);
	
};

#endif
