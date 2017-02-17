// MedianFilter.h - Moritz Hauff - 14.02.2017

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

#ifndef _MEDIANFILTER_h
#define _MEDIANFILTER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define ARRAYLENGTH 50

///////////////////////////////////////////////////////////////////////////
///KalmanFilterClass
/* Die MedianFilter-Class verwaltet Messwerte
und wendet auf diese einen eindimensionalen MedianFilter an.*/
class MedianFilterClass
{
protected:
	int TotalNumberOfValues;   // wie viele Werte insgesamt gespeichert werden.
	int measurements[ARRAYLENGTH];
	double sortedMeasurements[ARRAYLENGTH];
	int currentpos;
	bool ready;

public:
	/* Initialize a KalmanFilter with standard properties, which need to be specified for satifying results.*/
	MedianFilterClass();
	/* Initialize the correct formulas calculating the measurement update.*/
	void Init(int TotalNumberOfValues);

	/* Add a new measurement.*/
	void Update(int measurement);

	/* Get the current value.*/
	int GetValue();
	/* Get the current value converted to an integer.*/
	int GetValue_Int();

	/* Print current properties to serial terminal.*/
	void PrintProps();
};

#endif
