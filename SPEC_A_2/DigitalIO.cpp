/** DigitalIO.cpp
***
*** Diese Klasse stellt die schnellen IO-Funktionen als Lese oder Schreib-Objekt 
*** zur Verfügung. So kann auf die einzelnen Pins genauso wie auf andere Sensoren 
*** zugegriffen werden.
***
*** Moritz Hauff, 18.03.2017
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

void DigitalIOClass::Update()
{
	lastState = Read();
}

bool DigitalIOClass::GetLastState()
{
	return lastState;
}

///////////////////////////////////////////////////////////////////////////
///LED-Functions
void LEDClass::Toggle()
{
	state = !state;
	Write(state);
}
