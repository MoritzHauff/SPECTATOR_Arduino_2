/** MLX90614Class.cpp
***
*** Diese Klasse dient dem Überwachen eines MLX90614 Temperatursensor von Adafruit.
***
*** created by Moritz Hauff, 06.01.2017
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
#include "MLX90614Class.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MLX90614Class::MLX90614Class(byte Address)
	: _address(Address)
{
	mlx = Adafruit_MLX90614(_address);
	mlx.begin();
}


///////////////////////////////////////////////////////////////////////////
///Funktionen
float MLX90614Class::GetObjTemp()
{
	return lastObjTemp;
}

void MLX90614Class::Update()
{
	lastObjTemp = mlx.readObjectTempC();
}
