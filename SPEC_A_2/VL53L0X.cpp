/** VL53K0X.cpp
***
*** todo
***
*** created by Moritz Hauff, 04.04.2017
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
#include "VL53L0X.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void VL53L0XClass::Init()
{
	if (!lox.begin())   // todo: set sensor-Settings
	{   
		Serial.println(F("Failed to boot VL53L0X"));
	}

	lox.StartContiniousMeasurement(true);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
uint16_t VL53L0XClass::GetDistance()
{
	dataUpdated = false;

	return measure.RangeMilliMeter;
}

void VL53L0XClass::Update()
{
	if (lox.CheckMeasurementSucces(&measure, true))   // konstant 6800 us.
	{
		//Serial.print("Distance (mm): "); Serial.print(measure.RangeMilliMeter);
		dataUpdated = true;
		lox.StartContiniousMeasurement(true);
	}
}
