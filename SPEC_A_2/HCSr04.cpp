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
	return (PulsLaenge * 34) / 2000;
}

int HCSr04Class::GetDistance()
{
	return cm;
}
