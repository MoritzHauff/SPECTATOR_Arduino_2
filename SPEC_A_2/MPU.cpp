/** MPU.cpp
***
*** Die MPU-Class verwaltet greift auf ein MPU6050 zu und analysiert
*** dessen Werte.
*** Diese sollten auﬂerdem in Zukunft zur Weiterverwendung bereitgestellt werden.
***
*** Moritz Hauff - 30.12.2016
**/

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

///////////////////////////////////////////////////////////////////////////
///Includes
#include "MPU.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MPU::MPU()
{
	dmpReady = false;
	ypr_correction[0] = 0;
	ypr_correction[1] = 0;
	ypr_correction[2] = 0;
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
			mpu.getFIFOBytes(fifoBuffer, packetSize); //Die LeseZeit betr‰gt 1700 us.

			// track FIFO count here in case there is > 1 packet available
			// (this lets us immediately read more without waiting for an interrupt) - oder auch nicht. Es geschieht immer nur genau eine Aktualisierung pro Funktionsaufruf.
			fifoCount -= packetSize;

			// display Euler angles in degrees
			mpu.dmpGetQuaternion(&q, fifoBuffer);
			mpu.dmpGetGravity(&gravity, &q);
			mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  // Die gesamte Berechnung benˆtigt ca. 980 us. Das ist OK.
			
			Serial.print("\t\t\typr\t");
			Serial.print(ypr[0] - ypr_correction[0], 4);  // das Weglassen einer Umrechnung bringt nochmal 180 us.
			Serial.print("\t");
			Serial.print(ypr[1] - ypr_correction[1], 4);  // todo: Bias-Normalisierung auf 0 - 2pi
			Serial.print("\t");
			Serial.println(ypr[2] - ypr_correction[2], 4);  // Die Ausgabe benˆtigt ca. 1400 us. Das ist gerade so in Ordnung.

			//todo: Tue irgendetwas mit den Daten.
		}
	}
}

uint8_t MPU::WaitForCalibration(uint16_t Timeout)
{
	unsigned long startZeitpunkt = millis();
	unsigned long aktZeitpunkt = startZeitpunkt;
	float yawReference = ypr[0];

	while (startZeitpunkt + Timeout > millis())
	{
		if (dmpReady)
		{
			Update();
			if (!(yawReference + 0.0002 > ypr[0] && yawReference - 0.0002 < ypr[0]))  // Wert liegt nicht innerhalb einer gewissen Toleranz.
			{
				yawReference = ypr[0];
				aktZeitpunkt = millis();
			}
			if (aktZeitpunkt + 1000 < millis())  // Wert soll mind. 1 sec stabil sein.
			{
				ypr_correction[0] = ypr[0];
				ypr_correction[1] = ypr[1];
				ypr_correction[2] = ypr[2];

				Serial.print("MPU-Kalibrierung erfolgreich. Kalibrierungszeit: ");
				Serial.println(millis() - startZeitpunkt);
				Serial.println(" ms.");

				return CALIBRATION_SUCCESS;
			}
		}
	}
	return CALIBRATION_ERROR;
}