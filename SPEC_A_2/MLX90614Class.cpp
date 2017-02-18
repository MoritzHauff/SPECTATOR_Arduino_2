/** MLX90614Class.cpp
***
*** Diese Klasse dient dem Überwachen eines MLX90614 Temperatursensor von Adafruit.
***
*** created by Moritz Hauff, 06.01.2017
**/

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
	return mlx.readObjectTempC();
}