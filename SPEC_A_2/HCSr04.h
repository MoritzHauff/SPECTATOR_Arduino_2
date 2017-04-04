// HCSr04.h - Moritz Hauff - 04.04.2017

#ifndef _HCSR04_h
#define _HCSR04_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DigitalIO.h"

///////////////////////////////////////////////////////////////////////////
///HCSr04Class
/*Diese Klasse überwacht die HCSr04-Sensoren.*/
class HCSr04Class
{
 protected:
	 uint8_t _echo_Pin;
	 DigitalIOClass *trigPin;  // use the fast IO-functions.

	 long pulsLaenge;
	 int cm;

	 /*Converts the pulsLaenge into cm.*/
	 int convert(long PulsLaenge);

 public:
	 HCSr04Class(uint8_t Echo_Pin, GPIO_pin_t Trig_Pin);
	 ~HCSr04Class();
	 void Init();

	 /*Führt eine Messung aus und speichert diese in der Klasseninstanz.*/
	 void Update();
	 /*Gibt den aktuell gespeicherten Wert zurück.*/
	 int GetDistance();


};

#endif
