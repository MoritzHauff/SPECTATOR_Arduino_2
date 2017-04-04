// VL53L0X.h - Moritz Hauff - 04.04.2017

#ifndef _VL53L0X_h
#define _VL53L0X_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_VL53L0X.h>  // Es wird die angepasste Adafruit-Bibliothek verwendet.

///////////////////////////////////////////////////////////////////////////
///VL53L0X-Class
class VL53L0XClass
{
protected:
	bool dataUpdated = false;
	VL53L0X_RangingMeasurementData_t measure;

	Adafruit_VL53L0X lox = Adafruit_VL53L0X();

public:
	void Init();

	void Update();
	bool NewDataAvaiable() { return dataUpdated; }
	uint16_t GetDistance();

};

#endif
