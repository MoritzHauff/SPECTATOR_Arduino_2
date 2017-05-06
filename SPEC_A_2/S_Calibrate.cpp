/** S_Calibrate.cpp
***
*** Dieser State kalibriert das MPU6050.
***
*** Moritz Hauff, 06.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_Calibrate.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_CalibrateClass::Init()
{
	status = Finished;

	MPUCalibration();

	spectator->AktRichtung = R_NORDEN;  // nach dem kalibrierne ist die richtung immer norden (z.B. nach neustarts).
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_CalibrateClass::MPUCalibration()
{
	Serial.println("MPU6050-Kalibrierung...");
	if (spectator->mpu.WaitForCalibration(S_Calibrate_MPU_Timer) != CALIBRATION_SUCCESS)  // Rückgabewert kann zum Beispiel an Raspberry gesendet werden.
	{
		Serial.println("MPU6050-Kalibrierung gescheitert!");
		status = Error;
	}

	spectator->mpu.Update();
	Serial.print("aktueller Yaw-Wert: ");
	Serial.println(spectator->mpu.GetYaw());

	spectator->mpuFahrer.SetNorden(spectator->mpu.GetYaw());   // hardcode the mpuFahrerCalibration // todo: should normally done be the RaPi.

	Serial.println("MPU6050 kalibriert.");
}
