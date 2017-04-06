/** SPECMotoren.cpp
***
*** Steuert das Adafruit Motorshield.
***
*** Moritz Hauff, 01.03.2017
**/

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

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECMotoren.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
SPECMotorenClass::SPECMotorenClass(uint8_t MotorPortLinks, uint8_t MotorPortRechts, uint8_t RescueMotorPort, uint8_t UnterflurPort,
	uint8_t StepperLPin1, uint8_t StepperLPin2, uint8_t StepperLPin3, uint8_t StepperLPin4,
	uint8_t StepperRPin1, uint8_t StepperRPin2, uint8_t StepperRPin3, uint8_t StepperRPin4)
{
	MotorL = AFMS.getMotor(MotorPortLinks);		// Select which 'port' M1, M2, M3 or M4.
	MotorR = AFMS.getMotor(MotorPortRechts);
	RescueMotoren = AFMS.getMotor(RescueMotorPort);
	UnterflurBeleuchtung = AFMS.getMotor(UnterflurPort);


	// create an instance of the stepper class, specifying
	// the number of steps of the motor and the pins it's
	// attached to
	stepperL = new Stepper(STEPS, StepperLPin1, StepperLPin2, StepperLPin3, StepperLPin4);
	stepperR = new Stepper(STEPS, StepperRPin1, StepperRPin2, StepperRPin3, StepperRPin4);
}

void SPECMotorenClass::Init()
{
	Serial.println("Starte MotorShield.");
	AFMS.begin();  // create with the default frequency 1.6KHz
	Serial.println("MotorShield gestartet.");

	encoderLinks.Init();
	encoderRechts.Init();

	// set the speed of the motor to 60 RPMs
	stepperL->setSpeed(60);
	stepperR->setSpeed(60);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
bool SPECMotorenClass::SetMotorR(int Speed)
{
	if (Speed > 1 && Speed <= 255)
	{
		MotorR->run(FORWARD);
		lastDirectionR = FORWARD;
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorR->run(BACKWARD);
		Speed = -Speed;
		lastDirectionR = BACKWARD;
	}
	else if (Speed == 1 || Speed == 0)
	{
		MotorR->run(RELEASE);
	}
	else
	{
		Serial.println("MotorR speed out of bounds");
		return false;
	}
	MotorR->setSpeed(Speed);
	return true;
}

bool SPECMotorenClass::SetMotorL(int Speed)
{
	if (Speed > 1 && Speed <= 255)
	{
		MotorL->run(FORWARD);
		lastDirectionL = FORWARD;
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorL->run(BACKWARD);
		Speed = -Speed;
		lastDirectionL = BACKWARD;
	}
	else if (Speed == 1 || Speed == 0)
	{
		MotorL->run(RELEASE);
	}
	else
	{
		Serial.println("MotorL speed out of bounds");
		return false;
	}
	MotorL->setSpeed(Speed);
	return true;
}

bool SPECMotorenClass::SetMotoren(int SpeedL, int SpeedR)
{
	if (SetMotorL(SpeedL) && SetMotorR(SpeedR))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SPECMotorenClass::Kontrolllauf(void)
{
	SetMotorR(-150);
	SetMotorL(-150);
	//delay(1000);
	delay(100);

	SetMotorR(0);
	SetMotorL(0);
	/*delay(1000);

	LED.switchOn();
	RescueKitAbwerfen();*/
}

void SPECMotorenClass::UpdateWheelEncoderInfo()
{
	encoderLinks.Update();
	encoderRechts.Update();

	lastEncoderInfoL = encoderLinks.GetEncoderInfo();
	lastEncoderInfoR = encoderRechts.GetEncoderInfo();

	if (lastDirectionL == BACKWARD)
	{
		lastEncoderInfoL.CountsSinceLastTick = -(lastEncoderInfoL.CountsSinceLastTick);
	}
	if (lastDirectionR == BACKWARD)
	{
		lastEncoderInfoR.CountsSinceLastTick = -(lastEncoderInfoR.CountsSinceLastTick);
	}
}

void SPECMotorenClass::TurnLEDOn()
{
	if (PWR_LEDRichtung == FORWARD)
	{
		UnterflurBeleuchtung->run(FORWARD);
		UnterflurBeleuchtung->setSpeed(240);
	}
	if (PWR_LEDRichtung == BACKWARD)
	{
		UnterflurBeleuchtung->run(BACKWARD);
		UnterflurBeleuchtung->setSpeed(240);
	}
}

void SPECMotorenClass::TurnLEDOff()
{
	UnterflurBeleuchtung->run(RELEASE);
	UnterflurBeleuchtung->setSpeed(0);
}

void SPECMotorenClass::AbwurfLinks()
{
	stepperL->step(STEPS);
}

void SPECMotorenClass::StepL(float Revolutions)
{
	stepperL->step(STEPS*Revolutions);
}

void SPECMotorenClass::AbwurfRechts()
{
	stepperR->step(STEPS);
}

void SPECMotorenClass::StepR(float Revolutions)
{
	stepperR->step(STEPS*Revolutions);
}
