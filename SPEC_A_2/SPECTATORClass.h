// SPECTATORClass.h - Moritz Hauff - 01.03.2017

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

#ifndef _SPECTATORCLASS_h
#define _SPECTATORCLASS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DigitalIO.h"
#include "SharpIR.h"
#include "MPU.h"
#include "MLX90614Class.h"

#include "SPECMotoren.h"

#include "SerialBuffer.h"
#include "MPUFahrer.h"

#include "Functions.h"
#include "Constants.h"

///////////////////////////////////////////////////////////////////////////
///Constants
const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen würden damit 4 ms dauern.

///////////////////////////////////////////////////////////////////////////
///SPECTATORClass
/*Hier laufen alle anderen benötigten Instanzen und die dazugehörigen 
Funktionen zusammen. Die Klasse ist eine Singleton-Instanz.*/
class SPECTATORClass
{
 protected:
	 void MPUCalibration();

 public:
	void Init();

	void UpdateSwitches();
	/*Liest die vier Sharp-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateSharp();
	/*Liest die MLX-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateMLX();
	/*Liest die MPU-Werte aus und speichert diese sofern neue vorhanden sind im SerialBuffer.*/
	void UpdateMPU();

	LEDClass HeartbeatLED = LEDClass(DP32);
	//DigitalIOClass HeartbeatLED = DigitalIOClass(DP32, OUTPUT);

	DigitalIOClass switchLinks = DigitalIOClass(DP23, INPUT);
	DigitalIOClass switchRechts = DigitalIOClass(DP25, INPUT);

	SharpIR sharplinksvorne = SharpIR(A1, SHARPMEASUREMTS);
	SharpIR sharprechtsvorne = SharpIR(A2, SHARPMEASUREMTS);
	SharpIR sharplinkshinten = SharpIR(A3, SHARPMEASUREMTS);
	SharpIR sharprechtshinten = SharpIR(A4, SHARPMEASUREMTS);

	MPU mpu = MPU();

	MLX90614Class MLXLinks = MLX90614Class(0x2C);
	MLX90614Class MLXVorne = MLX90614Class(0x2A);
	MLX90614Class MLXRechts = MLX90614Class(0x2B);

	SPECMotorenClass Motoren = SPECMotorenClass(1, 2, 3, 4);

	SerialBuffer serialBuffer = SerialBuffer();
	MPUFahrerClass mpuFahrer = MPUFahrerClass();


	int zielRichtung;  // todo: this should be somewhere totally else!
	int MotorSpeedL;  // sollen diese Werte wirklcih hier gespeichert werden?
	int MotorSpeedR;
};

extern SPECTATORClass SA;

#endif
