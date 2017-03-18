// DigitalIO.h - Moritz Hauff - 18.03.2017

#ifndef _DIGITALIO_h
#define _DIGITALIO_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions

///////////////////////////////////////////////////////////////////////////
///DigitalIO-Class
/*Diese Klasse stellt die schnellen IO-Funktionen als Lese oder Schreib-Objekt 
zur Verfügung. So kann auf die einzelnen Pins genauso wie auf andere Sensoren 
zugegriffen werden.*/
class DigitalIOClass
{
 protected:
	 /*True if it is an output pin, 
	 False if it is an input pin.*/
	 bool isOutput;
	 /*The Pin refered by this object.*/
	 GPIO_pin_t pin;

	 bool lastState;

 public:
	 DigitalIOClass(const GPIO_pin_t Pin, const uint8_t PinMode);
	 /*Bereitet den Pin auf den Lese/Schreibzugriff vor.*/
	 void Init();

	 //is explicit inline - also not really faster
	 /*inline bool Read() const {
		 return digitalRead2f(pin);
	 }*/

	 /*Reads the current State of the Pin, if it's an input Pin.*/
	 bool Read();
	 /*Aquivalent of digital Write with the fast IO-functions.*/
	 void Write(uint8_t Value);
	 /*Aquivalent of digital Write for bool variables with the fast IO-functions.*/
	 void WriteBool(bool State);
	 /*Liest den aktuellen anliegenden Wert aus und speichert diesen in der Klasseninstanz.
	 Kann mithilfe GetLastState wieder ausgelesen werden.*/
	 void Update();

	 bool GetLastState();

};

/*Diese kleine vererbte Klasse ermöglicht das Steuern von LEDs.*/
class LEDClass : DigitalIOClass
{
protected:
	bool state;
public:
	LEDClass(const GPIO_pin_t Pin) : DigitalIOClass(Pin, OUTPUT)
	{
		state = false;
		Write(state);
	}

	using DigitalIOClass::Init;  // Nur die Init Funktion soll öffentlich sichtbar sein.

	void Toggle();
};

#endif
