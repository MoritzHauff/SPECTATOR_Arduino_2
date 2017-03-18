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

///////////////////////////////////////////////////////////////////////////
///Functions
bool DigitalIOClass::Read()  // todo: implement it with inline/sth else so that it is faster.
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

///////////////////////////////////////////////////////////////////////////
///LED-Functions
void LEDClass::Toggle()
{
	state = !state;
	Write(state);
}