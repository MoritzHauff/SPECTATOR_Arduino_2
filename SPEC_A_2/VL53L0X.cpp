/** VL53K0X.cpp
***
*** todo
***
*** created by Moritz Hauff, 04.04.2017
**/

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
	//dataUpdated = false;

	return measure.RangeMilliMeter;
}

void VL53L0XClass::Reset()
{
	dataUpdated = false;
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
