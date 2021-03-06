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
#include "LDR.h"
#include "SharpIR.h"
#include "HCSr04.h"
#include "MPU.h"
#include "MLX90614Class.h"
#include "VL53L0X.h"

#include "SPECMotoren.h"

#include "SerialBuffer.h"
#include "MPUFahrer.h"
#include "OpferKontrolleur.h"

#include "Functions.h"
#include "Constants.h"

///////////////////////////////////////////////////////////////////////////
///Constants
const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen w�rden damit 4 ms dauern.

#define R_VORNE 1
#define R_RECHTS 2
#define R_HINTEN 3
#define R_LINKS 4 

///////////////////////////////////////////////////////////////////////////
///SPECTATORClass
/*Hier laufen alle anderen ben�tigten Instanzen und die dazugeh�rigen 
Funktionen zusammen. Die Klasse ist eine Singleton-Instanz.*/
class SPECTATORClass
{
 protected:
	 
 public:
	void Init();

	/*Liest die zwei Switch-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateSwitches();
	/*Liest die vier Sharp-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateSharp();
	/*Liest die MLX-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateMLX();
	/*Liest die MPU-Werte aus und speichert diese sofern neue vorhanden sind im SerialBuffer.*/
	void UpdateMPU();

	void UpdateHCSr04Seitlich();

	void UpdateHCSr04VorneHinten();

	void UpdateLaser();

	void UpdateEncoder();

	void UpdateLDR();

	bool GetWand(byte Direction);

	LEDClass HeartbeatLED = LEDClass(DP13);

	DigitalIOClass switchLinks = DigitalIOClass(DP53, INPUT); // todo: PascalCase
	DigitalIOClass switchRechts = DigitalIOClass(DP51, INPUT);

	LDR ldr = LDR(A11);

	SharpIR sharplinksvorne = SharpIR(A12, SHARPMEASUREMTS);
	SharpIR sharplinkshinten = SharpIR(A13, SHARPMEASUREMTS);
	SharpIR sharprechtshinten = SharpIR(A14, SHARPMEASUREMTS);
	SharpIR sharprechtsvorne = SharpIR(A15, SHARPMEASUREMTS);

	HCSr04Class ultraschallLinks = HCSr04Class(43, DP41);
	HCSr04_InterruptClass ultraschallVorne = HCSr04_InterruptClass(19, DP22);
	HCSr04Class ultraschallRechts = HCSr04Class(47, DP45);
	HCSr04_InterruptClass ultraschallHinten = HCSr04_InterruptClass(18, DP24);

	VL53L0XClass laserVorne = VL53L0XClass();

	MPU mpu = MPU();

	MLX90614Class MLXLinks = MLX90614Class(0x2C);
	MLX90614Class MLXVorne = MLX90614Class(0x2A);
	MLX90614Class MLXRechts = MLX90614Class(0x2B);

	SPECMotorenClass Motoren = SPECMotorenClass(3, 1, 4, 2,
		4, 5, 6, 7,
		11, 10, 9, 8);

	SerialBuffer serialBuffer = SerialBuffer();
	MPUFahrerClass mpuFahrer = MPUFahrerClass();

	OpferKontrolleurClass OpferKontroller = OpferKontrolleurClass();

	byte AktRichtung;
	bool GeradeSchwarzesFeldBefahren;
	bool GeradeRampeBefahren;
	bool GeradeSonstWieNichtVorangekommen;

};

extern SPECTATORClass SA;

#endif
