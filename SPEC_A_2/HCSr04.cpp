/** HCSr04.cpp
***
*** Diese Klasse überwacht die HCSr04-Sensoren.
***
*** created by Moritz Hauff, 04.04.2017
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
#include "HCSr04.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
HCSr04Class::HCSr04Class(const uint8_t Echo_Pin, const GPIO_pin_t Trig_Pin)
{
	_echo_Pin = Echo_Pin;
	trigPin = new DigitalIOClass(Trig_Pin, OUTPUT);
}

HCSr04Class::~HCSr04Class()
{
	delete trigPin;
}

void HCSr04Class::Init()
{
	pinMode(_echo_Pin, INPUT);
	trigPin->Init();
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void HCSr04Class::Update()
{
	trigPin->Write(LOW);
	delayMicroseconds(2);
	trigPin->Write(HIGH);
	delayMicroseconds(5);
	trigPin->Write(LOW);

	pulsLaenge = pulseIn(_echo_Pin, HIGH);

	cm = convert(pulsLaenge);
}

int HCSr04Class::convert(long PulsLaenge)
{
	return PulsLaenge / 58;   // only do one int-Calculation saves time.
}

int HCSr04Class::GetDistance()
{
	return cm;
}


HCSr04_InterruptClass *HCSr04_InterruptClass::_instance(NULL);

HCSr04_InterruptClass::HCSr04_InterruptClass(int trigger, int echo, int max_dist)
	: _trigger(trigger), _echo(echo), _max(max_dist), _finished(false),
	HCSr04Class(trigger, echo)
{
	if (_instance == 0) _instance = this;
}

void HCSr04_InterruptClass::begin() 
{
	pinMode(_trigger, OUTPUT);
	digitalWrite(_trigger, LOW);
	pinMode(_echo, INPUT);
	attachInterrupt(digitalPinToInterrupt(_echo), _echo_isr, CHANGE);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void HCSr04_InterruptClass::start() 
{
	_finished = false;
	digitalWrite(_trigger, HIGH);
	delayMicroseconds(15);
	digitalWrite(_trigger, LOW);
}

unsigned int HCSr04_InterruptClass::getDistance() 
{
	return convert(_end - _start);
}

void HCSr04_InterruptClass::_echo_isr() 
{
	HCSr04_InterruptClass* _this = HCSr04_InterruptClass::instance();

	switch (digitalRead(_this->_echo)) 
	{
	case HIGH:
		_this->_start = micros();
		break;
	case LOW:
		_this->_end = micros();
		_this->_finished = true;
		break;
	}
}
