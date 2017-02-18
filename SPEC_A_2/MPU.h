// MPU.h - Moritz Hauff - 17.02.2017

#ifndef _MPU_h
#define _MPU_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MPU6050_2.h" // import *new* MPU6050.h with MotionApps already included

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

///////////////////////////////////////////////////////////////////////////
///Konstanten
const uint8_t CALIBRATION_SUCCESS = 1;
const uint8_t CALIBRATION_ERROR = 0;

///////////////////////////////////////////////////////////////////////////
///MPU-Class
/*Diese Klasse stellt die Daten eines MPU6050 zur Verfügung.*/
class MPU
{
protected: 
	MPU6050 mpu = MPU6050();

	// MPU control/status vars
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer

	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float ypr[3];           // [yaw, pitch, roll]   raw yaw/pitch/roll container and gravity vector
	float ypr_correction[3];// yaw/pitch/roll container for offset-correction
	float normalisedypr[3];

	bool DataUpdated = false; 

public: 
	MPU();
	/*Initalisiert den MPU6050. Gibt den Fortschritt über die serielle Schnittstelle aus.*/
	void Init();
	/*Überpüft ob dem MPU neue Daten vorliegen und übernimmt diese gegebenfalls.
	Dauer: 5169 us wenn neue Daten vorliegen, sonst 200 us.*/	
	void Update();
	/*Führt eine Kalibrierung des MPU durchs. Wartet bis sich die Werte stabilisiert haben und
	stellt anhand dessen dann das Offset ein. Bricht nach einem gewissen Timeout (ms) die Fkt. ab falls sie zu lange kein Ergebnis liefert.*/
	uint8_t WaitForCalibration(uint16_t Timeout);

	/*Gibt an ob seit dem letzten Auslesen der Ýaw/Pitch/Roll-Werte diese aktualisiert wurden.*/
	bool NewDataAvaible();
	float GetYaw();
	float GetPitch();
	float GetRoll();
};

#endif

