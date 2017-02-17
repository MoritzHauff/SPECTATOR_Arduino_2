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
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

public: 
	MPU();
	/*Initalisiert den MPU6050. Gibt den Fortschritt über die serielle Schnittstelle aus.*/
	void Init();
	/*Überpüft ob dem MPU neue Daten vorliegen und übernimmt diese gegebenfalls.
	Dauer: 5169 us wenn neue Daten vorliegen, sonst 200 us.*/	
	void Update();
};

#endif

