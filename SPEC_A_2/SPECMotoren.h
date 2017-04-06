// SPECMotoren.h - Moritz Hauff - 02.03.2017

///////////////////////////////////////////////////////////////////////////
/// Copyright (C) {2017}  {Moritz Hauff}
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// If you start using parts of this code please write a short message to: 
/// license@vierradroboter.de
///
/// If you have any questions contact me via mail: admin@vierradroboter.de
///////////////////////////////////////////////////////////////////////////

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
const int PWR_MotorL = BACKWARD;
const int PWR_MotorR = FORWARD;
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

	 EncoderInfo lastEncoderInfoL;
	 uint8_t lastDirectionL;  // contains the last set direction to the left Motor.
	 EncoderInfo lastEncoderInfoR;
	 uint8_t lastDirectionR;  // contains the last set direction to the right Motor.

	 bool SetMotorL(int Speed);
	 bool SetMotorR(int Speed);

 public:
	SPECMotorenClass(uint8_t MotorPortLinks, uint8_t MotorPortRechts, uint8_t RescueMotorPort, uint8_t UnterflurPort, 
		uint8_t StepperLPin1, uint8_t StepperLPin2, uint8_t StepperLPin3, uint8_t StepperLPin4,
		uint8_t StepperRPin1, uint8_t StepperRPin2, uint8_t StepperRPin3, uint8_t StepperRPin4);
	void Init();

	WheelEncoderClass encoderLinks;
	WheelEncoderClass encoderRechts;

	/*Aktualisiert beide WheelEncoderInformationen und versieht diese mit der aktuellen Drehrichtung.*/
	void UpdateWheelEncoderInfo();
	/*Gibt die linken EncoderInforamtionen zurück.*/
	EncoderInfo GetEncoderInfoL() { return lastEncoderInfoL; }
	/*Gibt die rechten EncoderInforamtionen zurück.*/
	EncoderInfo GetEncoderInfoR() { return lastEncoderInfoR; }

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
