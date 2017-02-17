/** MPU.cpp
***
*** Die MPU-Class verwaltet greift auf ein MPU6050 zu und analysiert
*** dessen Werte.
*** Diese sollten au�erdem in Zukunft zur Weiterverwendung bereitgestellt werden.
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
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
		mpuIntStatus = mpu.getIntStatus();

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else {
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
	if (!dmpReady) return;

	// reset interrupt flag and get INT_STATUS byte
	//mpuIntStatus = mpu.getIntStatus();

	// get current FIFO count
	fifoCount = mpu.getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		// reset so we can continue cleanly
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));

		// otherwise, check for DMP data ready interrupt (this should happen frequently)
	}
	else if(fifoCount > packetSize)/*if (mpuIntStatus & 0x02) */
	{
		// wait for correct available data length, should be a VERY short wait
		unsigned long eins = micros();
		//while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		unsigned long zwei = micros();
		Serial.print("Wartezeit: ");
		Serial.print(zwei - eins);  //Die Wartezeit betrug ca. 6000us. Das ist zu viel.

		eins = micros();
		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		zwei = micros();
		Serial.print(" Lesen: ");
		Serial.print(zwei - eins);  //Die LeseZeit betr�gt 1700 us.

		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;

		// display Euler angles in degrees
		eins = micros();
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		zwei = micros();
		Serial.print(" Berechnung: ");  // Die Berechnung ben�tigt ca. 980 us. Das ist OK.
		Serial.println(zwei - eins);
		eins = micros();
		Serial.print("ypr\t");
		Serial.print(ypr[0]/* * 180 / M_PI*/);  // das Weglassen der Umrechnung bringt nochmal 180 us.
		Serial.print("\t");
		Serial.print(ypr[1]/* * 180 / M_PI*/);
		Serial.print("\t");
		Serial.println(ypr[2]/* * 180 / M_PI*/);
		zwei = micros();
		Serial.print(" Ausgabe: ");  // Die Ausgabe ben�tigt ca. 1400 us. Das ist gerade so in Ordnung.
		Serial.print(zwei - eins);
	}

}