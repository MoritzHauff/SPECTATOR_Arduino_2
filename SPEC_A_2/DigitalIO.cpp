/** DigitalIO.cpp
***
*** Diese Klasse stellt die schnellen IO-Funktionen als Lese oder Schreib-Objekt 
*** zur Verfügung. So kann auf die einzelnen Pins genauso wie auf andere Sensoren 
*** zugegriffen werden.
***
*** Moritz Hauff, 18.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "DigitalIO.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
DigitalIOClass::DigitalIOClass(const GPIO_pin_t Pin, const uint8_t PinMode)
{
	pin = Pin;
	isOutput = PinMode;
}

void DigitalIOClass::Init()
{
	pinMode2f(pin, isOutput);
}

bool DigitalIOClass::Read()  // is inline faster?
{
	return digitalRead2f(pin);
}

void DigitalIOClass::Write(uint8_t Value)
{
	digitalWrite2f(pin, Value);
}

void DigitalIOClass::WriteBool(bool State)
{
	digitalWrite2f(pin, State);
}
