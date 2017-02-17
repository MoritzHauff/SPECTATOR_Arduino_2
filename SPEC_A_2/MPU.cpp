/** MPU.cpp
***
*** Die MPU-Class verwaltet greift auf ein MPU6050 zu und analysiert
*** dessen Werte.
*** Diese sollten außerdem in Zukunft zur Weiterverwendung bereitgestellt werden.
***
*** Moritz Hauff - 30.12.2016
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "MPU.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MPU::MPU()
{
	dmpReady = false;
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void MPU::Init()
{
	// initialize device
	Serial.println(F("Initializing MPU-I2C ..."));
	mpu.initialize();

	// verify connection
	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

	//load and configure the DMP
	Serial.println(F("Initializing DMP..."));
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);   // todo: Hier richtige Kalibrierung.
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) 
	{
		// turn on the DMP, now that it's ready
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		Serial.println(F("DMP ready! Waiting for first update()-Call..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else 
	{
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
}

void MPU::Update()
{
	// if programming failed, don't try to do anything
	if (dmpReady)
	{
		// get current FIFO count
		fifoCount = mpu.getFIFOCount();

		// check for overflow (this should never happen unless our code is too inefficient)
		if (fifoCount == 1024) 
		{
			// reset so we can continue cleanly
			mpu.resetFIFO();
			Serial.println(F("FIFO overflow!"));
		}
		else if (fifoCount > packetSize)
		{
			// read a packet from FIFO
			mpu.getFIFOBytes(fifoBuffer, packetSize); //Die LeseZeit beträgt 1700 us.

			// track FIFO count here in case there is > 1 packet available
			// (this lets us immediately read more without waiting for an interrupt) - oder auch nicht. Es geschieht immer nur genau eine Aktualisierung pro Funktionsaufruf.
			fifoCount -= packetSize;

			// display Euler angles in degrees
			mpu.dmpGetQuaternion(&q, fifoBuffer);
			mpu.dmpGetGravity(&gravity, &q);
			mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  // Die gesamte Berechnung benötigt ca. 980 us. Das ist OK.
			
			Serial.print("\t\t\typr\t");
			Serial.print(ypr[0]/* * 180 / M_PI*/);  // das Weglassen der Umrechnung bringt nochmal 180 us.
			Serial.print("\t");
			Serial.print(ypr[1]/* * 180 / M_PI*/);
			Serial.print("\t");
			Serial.println(ypr[2]/* * 180 / M_PI*/);  // Die Ausgabe benötigt ca. 1400 us. Das ist gerade so in Ordnung.

			//todo: Tue irgendetwas mit den Daten.
		}
	}
}