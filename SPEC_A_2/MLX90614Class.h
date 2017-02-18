// MLX90614Class.h - Moritz Hauff - 06.01.2017

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
/* Diese Klasse dient dem �berwachen eines MLX90614 Temperatursensor von Adafruit.*/
class MLX90614Class
{
 protected:
	 byte _address;
	 Adafruit_MLX90614 mlx;

 public:
	 MLX90614Class(byte Address = 0x5A);
	 /*Returns the object temperature in �C.*/
	 float GetObjTemp();
	 
};

#endif
