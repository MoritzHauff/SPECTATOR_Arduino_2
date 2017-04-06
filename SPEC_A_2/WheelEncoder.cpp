/** WheelEncoder.cpp
***
*** todo
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
#include "WheelEncoder.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void WheelEncoderClass::Init()
{
	count = 0;

}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void WheelEncoderClass::HandleInterrupt()
{
	count++;
}

int WheelEncoderClass::GetCount()
{
	return count;
}

EncoderInfo WheelEncoderClass::GetEncoderInfo()
{
	return LastEncoderInfo;
}

void WheelEncoderClass::Update()
{
	LastEncoderInfo.MillisSinceLastTick = millis() - lastTime;
	LastEncoderInfo.CountsSinceLastTick = count;

	count = 0;
	lastTime = millis();
}

void WheelEncoderClass::PrintEncoderInfo(EncoderInfo EncoderInfo)
{
	Serial.print("vergangene Zeit [ms]: ");
	Serial.print(EncoderInfo.MillisSinceLastTick);
	Serial.print(" - Counts: ");
	Serial.println(EncoderInfo.CountsSinceLastTick);
}
