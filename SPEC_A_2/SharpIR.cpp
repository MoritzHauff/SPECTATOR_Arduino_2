/** SharpIR.cpp
***
*** Diese Klasse verarbeitet die Entfernungsdaten aus SharpIR-Sensoren.
***
*** Moritz Hauff - 15.06.2017
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
#include "SharpIR.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
SharpIR::SharpIR(uint8_t ir_Pin, uint8_t NumberOfValues)
{
    _ir_Pin=ir_Pin;
    _numberOfValues=NumberOfValues;
    
    // Define pin as Input
    pinMode (_ir_Pin, INPUT);
    
	median.Init(NumberOfValues);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
int SharpIR::GetValue()
{
	return median.GetValue();
}

int SharpIR::CalculateValue()
{
	for (int i = 0; i < _numberOfValues; i++)
	{
		median.Update(analogRead(_ir_Pin));
	}
	return median.GetValue();
}

void SharpIR::Update()
{
	median.Update(analogRead(_ir_Pin));
}

/*
	Ideas from "SharpIR"

	Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK

	From an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)

	Version : 1.0 : Guillaume Rico
	+ Remove average and use median
	+ Definition of number of sample in .h
	+ Define IR pin as input

	Version : 1.1 : Thibaut Mauon
	+ Add SHARP GP2Y0A710K0F for 100cm to 500cm by Thibaut Mauron

	https://github.com/guillaume-rico/SharpIR

	Original comment from Dr. Marcal Casas-Cartagena :
	The Sahrp IR sensors are cheap but somehow unreliable. I've found that when doing continous readings to a
	fix object, the distance given oscilates quite a bit from time to time. For example I had an object at
	31 cm. The readings from the sensor were mainly steady at the correct distance but eventually the distance
	given dropped down to 25 cm or even 16 cm. That's quite a bit and for some applications it is quite
	unacceptable. I checked the library http://code.google.com/p/gp2y0a21yk-library/ by Jeroen Doggen
	(jeroendoggen@gmail.com) and what the author was doing is to take a bunch of readings and give an average of them

	The present library works similary. It reads a bunch of readings (avg), it checks if the current reading
	differs a lot from the previous one (tolerance) and if it doesn't differ a lot, it takes it into account
	for the mean distance.
	The distance is calculated from a formula extracted from the graphs on the sensors datasheets
	After some tests, I think that a set of 20 to 25 readings is more than enough to get an accurate distance
	Reading 25 times and return a mean distance takes 53 ms. For my application of the sensor is fast enough.
	This library has the formulas to work with the GP2Y0A21Y and the GP2Y0A02YK sensors but exanding it for
	other sensors is easy enough.
*/
