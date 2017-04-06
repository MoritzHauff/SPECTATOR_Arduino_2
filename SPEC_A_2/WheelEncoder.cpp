/** WheelEncoder.cpp
***
*** todo
***
*** Moritz Hauff, 06.04.2017
**/

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
