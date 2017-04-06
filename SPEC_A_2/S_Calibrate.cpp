/** S_Calibrate.cpp
***
*** Dieser State kalibriert das MPU6050.
***
*** Moritz Hauff, 06.04.2017
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
#include "S_Calibrate.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_CalibrateClass::Init()
{
	status = Finished;

	MPUCalibration();

	
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_CalibrateClass::MPUCalibration()
{
	Serial.println("MPU6050-Kalibrierung...");
	if (spectator->mpu.WaitForCalibration(S_Calibrate_MPU_Timer) != CALIBRATION_SUCCESS)  // R�ckgabewert kann zum Beispiel an Raspberry gesendet werden.
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
