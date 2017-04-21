/** LDR.cpp
***
*** Verwaltet einen Helligkeitssensor.
***
*** Moritz Hauff - 21.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "LDR.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
LDR::LDR(uint8_t LDR_Pin)
{
	_LDR_Pin = LDR_Pin;
}

void LDR::Init()
{
	pinMode(_LDR_Pin, INPUT);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
int LDR::GetValue()
{
	return _lastValue;
}

void LDR::Update()
{
	_lastValue = analogRead(_LDR_Pin);  // todo use a faster function ?
}
