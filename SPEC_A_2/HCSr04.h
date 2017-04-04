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
	 bool dataUpdated = false;

	 /*Converts the pulsLaenge into cm.*/
	 int convert(long PulsLaenge);
	 /*Activates the HCSr04 and sends one "ping". Thereafter you need to analyse the Response of the sensor.*/
	 void ping();

 public:
	 HCSr04Class(const uint8_t Echo_Pin, const GPIO_pin_t Trig_Pin);
	 ~HCSr04Class();
	 void Init();

	 /*Führt eine Messung aus und speichert diese in der Klasseninstanz.*/
	 virtual void Update();
	 /*Gibt den aktuell gespeicherten Wert zurück.*/
	 int GetDistance();
	 /*Gibt an ob seit dem letzten Auslesen der Entfernungs-Wert aktualisiert wurden.*/
	 bool NewDataAvaible();

};

class HCSr04_InterruptClass : public HCSr04Class
{
protected:
	int _max;
	volatile unsigned long _start, _end;
	volatile bool _finished;

	void startMeasurement();
	

public:
	HCSr04_InterruptClass(const uint8_t Echo_Pin, const GPIO_pin_t Trig_Pin, int max_dist = 200);
	//~HCSr04_InterruptClass();

	virtual void Update();

	void HandleInterrupt();
	int GetEchoPin() { return _echo_Pin; }
};

#endif
