// SPECTATORClass.h - Moritz Hauff - 01.03.2017

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

	/*Liest die zwei Switch-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateSwitches();
	/*Liest die vier Sharp-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateSharp();
	/*Liest die MLX-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateMLX();
	/*Liest die MPU-Werte aus und speichert diese sofern neue vorhanden sind im SerialBuffer.*/
	void UpdateMPU();

	LEDClass HeartbeatLED = LEDClass(DP32);

	DigitalIOClass switchLinks = DigitalIOClass(DP23, INPUT); // todo: PascalCase
	DigitalIOClass switchRechts = DigitalIOClass(DP25, INPUT);

	SharpIR sharplinksvorne = SharpIR(A12, SHARPMEASUREMTS);
	SharpIR sharprechtsvorne = SharpIR(A13, SHARPMEASUREMTS);
	SharpIR sharplinkshinten = SharpIR(A14, SHARPMEASUREMTS);
	SharpIR sharprechtshinten = SharpIR(A15, SHARPMEASUREMTS);

	MPU mpu = MPU();

	MLX90614Class MLXLinks = MLX90614Class(0x2C);
	MLX90614Class MLXVorne = MLX90614Class(0x2A);
	MLX90614Class MLXRechts = MLX90614Class(0x2B);

	SPECMotorenClass Motoren = SPECMotorenClass(3, 2, 1, 4);

	SerialBuffer serialBuffer = SerialBuffer();
	MPUFahrerClass mpuFahrer = MPUFahrerClass();

};

extern SPECTATORClass SA;

#endif
