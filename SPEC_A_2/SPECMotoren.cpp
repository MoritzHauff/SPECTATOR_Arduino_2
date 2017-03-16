/** SPECMotoren.cpp
***
*** Steuert das Adafruit Motorshield.
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECMotoren.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
SPECMotorenClass::SPECMotorenClass(uint8_t MotorPortLinks, uint8_t MotorPortRechts, uint8_t RescueMotorPort, uint8_t UnterflurPort)
{
	MotorL = AFMS.getMotor(MotorPortLinks);		// Select which 'port' M1, M2, M3 or M4.
	MotorR = AFMS.getMotor(MotorPortRechts);
	RescueMotoren = AFMS.getMotor(RescueMotorPort);
	UnterflurBeleuchtung = AFMS.getMotor(UnterflurPort);
}

void SPECMotorenClass::Init()
{
	Serial.print("Starte MotorShield.");
	AFMS.begin();  // create with the default frequency 1.6KHz
	Serial.print("MotorShield gestartet.");
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
bool SPECMotorenClass::SetMotorR(int Speed)
{
	if (Speed > 1 && Speed <= 255)
	{
		MotorR->run(FORWARD);
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorR->run(BACKWARD);
		Speed = -Speed;
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
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorL->run(BACKWARD);
		Speed = -Speed;
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

void SPECMotorenClass::TurnRescueOn()
{
	if (PWR_LEDRichtung == FORWARD)
	{
		RescueMotoren->run(FORWARD);
		RescueMotoren->setSpeed(240);
	}
	if (PWR_LEDRichtung == BACKWARD)
	{
		RescueMotoren->run(BACKWARD);
		RescueMotoren->setSpeed(240);
	}
}

void SPECMotorenClass::TurnRescueOff()
{
	RescueMotoren->run(RELEASE);
	RescueMotoren->setSpeed(0);
}
