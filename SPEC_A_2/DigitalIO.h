// DigitalIO.h - Moritz Hauff - 17.03.2017

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
	 inline bool Read();
	 /*Aquivalent of digital Write with the fast IO-functions.*/
	 inline void Write(uint8_t Value);
	 /*Aquivalent of digital Write for bool variables with the fast IO-functions.*/
	 inline void WriteBool(bool State);
};

#endif
