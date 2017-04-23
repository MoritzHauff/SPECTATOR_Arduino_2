// MLX90614Class.h - Moritz Hauff - 06.01.2017

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

#ifndef _MLX90614CLASS_h
#define _MLX90614CLASS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <Adafruit_MLX90614.h>

///////////////////////////////////////////////////////////////////////////
///MLX90614Class
/* Diese Klasse dient dem Überwachen eines MLX90614 Temperatursensor von Adafruit.*/
class MLX90614Class
{
 protected:
	 byte _address;
	 Adafruit_MLX90614 mlx;

	 float lastObjTemp;

 public:
	 MLX90614Class(byte Address = 0x5A);
	 /*Reads a new Object Temperatur from the Sensor; Wire must be set to max 100KHz!*/
	 void Update();
	 /*Returns the last object temperature in °C.*/
	 float GetObjTemp();
	 
};

#endif
