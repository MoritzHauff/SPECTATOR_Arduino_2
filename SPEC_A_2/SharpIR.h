// SharpIR.h - Moritz Hauff - 15.06.2017

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

/*
	Ideas from Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK,

	an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)     
	
    Version : 1.0 : Guillaume Rico

	https://github.com/guillaume-rico/SharpIR
*/

#ifndef SharpIR_h
#define SharpIR_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MedianFilter.h"

///////////////////////////////////////////////////////////////////////////
///Constans
#define NB_SAMPLE 10

///////////////////////////////////////////////////////////////////////////
///SharpIR-Class
class SharpIR
{
private:
	MedianFilterClass median = MedianFilterClass();

	uint8_t _ir_Pin;
	uint8_t _numberOfValues;

public:

	SharpIR(uint8_t ir_Pin, uint8_t NumberOfValues);
    
	/*Gibt den median-Wert der letzten Updates zurück.*/
	int GetValue();

	/*Führt die angegebene Anzahl an Messungen durch und gibt dann den berechneten Wert zurück. Zeitkritisch!*/
	int CalculateValue();

	/*Führt eine einzelne Messung durch.*/
	void Update();

};

#endif
