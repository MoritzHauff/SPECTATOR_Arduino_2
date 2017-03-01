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
	if (Speed > 0 && Speed <= 255)
	{
		MotorR->run(FORWARD);
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorR->run(BACKWARD);
		Speed = -Speed;
	}
	else if (Speed == 0)
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
	if (Speed > 0 && Speed <= 255)
	{
		MotorL->run(FORWARD);
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorL->run(BACKWARD);
		Speed = -Speed;
	}
	else if (Speed == 0)
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
