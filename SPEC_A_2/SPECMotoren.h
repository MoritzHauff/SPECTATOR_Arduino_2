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
};

#endif
