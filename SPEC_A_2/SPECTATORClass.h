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

#include <Adafruit_MotorShield.h>

#include "SharpIR.h"
#include "MPU.h"
#include "MLX90614Class.h"

#include "SerialBuffer.h"

#include "Functions.h"

///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur f�r ihren im Setup angegeben Zweck nutzen, sonst kann es zur Besch�digung des ATmegas2560 kommen!
const GPIO_pin_t switchLinks_Pin = DP23;
const GPIO_pin_t switchRechts_Pin = DP25;

const uint8_t analog_Pin = A1;

const uint8_t DELAY_TIME = 100;

const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen w�rden damit 4 ms dauern.

const int Length_of_SerialMessage = 13;
const int length_of_information = 5;

///////////////////////////////////////////////////////////////////////////
///SPECTATORClass
/*Hier laufen alle anderen ben�tigten Instanzen und die dazugeh�rigen 
Funktionen zusammen. Die Klasse ist eine Singleton-Instanz.*/
class SPECTATORClass
{
 protected:


 public:
	void Init();

	SharpIR sharplinksvorne = SharpIR(analog_Pin, SHARPMEASUREMTS);
	SharpIR sharprechtsvorne = SharpIR(A2, SHARPMEASUREMTS);
	SharpIR sharplinkshinten = SharpIR(A3, SHARPMEASUREMTS);
	SharpIR sharprechtshinten = SharpIR(A4, SHARPMEASUREMTS);

	MPU mpu = MPU();

	MLX90614Class MLXLinks = MLX90614Class(0x2C);
	MLX90614Class MLXVorne = MLX90614Class(0x2A);
	MLX90614Class MLXRechts = MLX90614Class(0x2B);

	SerialBuffer serialBuffer = SerialBuffer();

};

extern SPECTATORClass SA;

#endif
