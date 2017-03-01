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

#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions

#include "SharpIR.h"
#include "MPU.h"
#include "MLX90614Class.h"

#include "SPECMotoren.h"

#include "SerialBuffer.h"

#include "Functions.h"
#include "Constants.h"

///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur für ihren im Setup angegeben Zweck nutzen, sonst kann es zur Beschädigung des ATmegas2560 kommen!
const GPIO_pin_t switchLinks_Pin = DP23;
const GPIO_pin_t switchRechts_Pin = DP25;

const uint8_t analog_Pin = A1;

const uint8_t DELAY_TIME = 100;

const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen würden damit 4 ms dauern.

const int Length_of_SerialMessage = 13;
const int length_of_information = 5;

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

	/*Liest die vier Sharp-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateSharp();
	/*Liest die MLX-Werte aus und speichert diese im SerialBuffer.*/
	void UpdateMLX();
	/*Liest die MPU-Werte aus und speichert diese sofern neue vorhanden sind im SerialBuffer.*/
	void UpdateMPU();

	SharpIR sharplinksvorne = SharpIR(analog_Pin, SHARPMEASUREMTS);
	SharpIR sharprechtsvorne = SharpIR(A2, SHARPMEASUREMTS);
	SharpIR sharplinkshinten = SharpIR(A3, SHARPMEASUREMTS);
	SharpIR sharprechtshinten = SharpIR(A4, SHARPMEASUREMTS);

	MPU mpu = MPU();

	MLX90614Class MLXLinks = MLX90614Class(0x2C);
	MLX90614Class MLXVorne = MLX90614Class(0x2A);
	MLX90614Class MLXRechts = MLX90614Class(0x2B);

	SPECMotorenClass Motoren = SPECMotorenClass(1, 2, 3, 4);

	SerialBuffer serialBuffer = SerialBuffer();

};

extern SPECTATORClass SA;

#endif
