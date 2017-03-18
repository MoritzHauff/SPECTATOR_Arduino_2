// DigitalIO.h - Moritz Hauff - 17.03.2017

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

 public:
	 DigitalIOClass(const GPIO_pin_t Pin, const uint8_t PinMode);
	 /*Bereitet den Pin auf den Lese/Schreibzugriff vor.*/
	 void Init();

	 /*Reads the current State of the Pin, if it's an input Pin.*/
	 bool Read();
	 /*Aquivalent of digital Write with the fast IO-functions.*/
	 void Write(uint8_t Value);
	 /*Aquivalent of digital Write for bool variables with the fast IO-functions.*/
	 void WriteBool(bool State);
};

#endif
