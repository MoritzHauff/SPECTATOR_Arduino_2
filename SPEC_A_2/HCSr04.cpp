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
	trigPin->Init();
	trigPin->Write(LOW);
	pinMode(_echo_Pin, INPUT);
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void HCSr04Class::ping()
{
	trigPin->Write(LOW);
	delayMicroseconds(2);
	trigPin->Write(HIGH);
	delayMicroseconds(5);
	trigPin->Write(LOW);

	//Serial.print("Ping gesendet.");
}

void HCSr04Class::Update()
{
	ping();

	pulsLaenge = pulseIn(_echo_Pin, HIGH);

	cm = convert(pulsLaenge);
}

int HCSr04Class::convert(long PulsLaenge)
{
	return PulsLaenge / 58;   // only do one int-Calculation saves time.  / 58
}

int HCSr04Class::GetDistance()
{
	return cm;
}

///////////////////////////////////////////////////////////////////////////
///Iterrupt-Konstruktoren
HCSr04_InterruptClass::HCSr04_InterruptClass(const uint8_t Echo_Pin, const GPIO_pin_t Trig_Pin, int max_dist)
	: _max(max_dist), _finished(true),
	HCSr04Class(Echo_Pin, Trig_Pin)
{ }

/*HCSr04_InterruptClass::~HCSr04_InterruptClass()
{
	~HCSr04Class();
}*/

/*void HCSr04_InterruptClass::Init() 
{
	pinMode(_trigger, OUTPUT);
	digitalWrite(_trigger, LOW);
	pinMode(_echo, INPUT);
	attachInterrupt(digitalPinToInterrupt(_echo), _echo_isr, CHANGE);
}*/

///////////////////////////////////////////////////////////////////////////
///Iterrupt-Funktionen
void HCSr04_InterruptClass::StartMeasurement()
{
	//Serial.println("Messung gestartet.");
	_finished = false;
	ping();
}

void HCSr04_InterruptClass::HandleInterrupt() 
{
	switch (digitalRead(_echo_Pin))
	{
	case HIGH:
		_start = micros();
		break;
	case LOW:
		if (_finished == false)
		{
			_end = micros();
			_finished = true;
			cm = convert(_end - _start);
		}
		break;
	}
}
