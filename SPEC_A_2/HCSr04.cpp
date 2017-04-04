/** HCSr04.cpp
***
*** Diese Klasse überwacht die HCSr04-Sensoren.
***
*** created by Moritz Hauff, 04.04.2017
**/

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
