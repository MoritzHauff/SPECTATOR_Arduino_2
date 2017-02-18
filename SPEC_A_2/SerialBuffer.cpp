/** SerialBuffer.cpp
***
*** Die SerialBuffer-Klasse verpackt Sensordaten in ein Übertragungsformat
*** und schickt sie über den seriellen Port an den RaspberryPi.
***
*** Moritz Hauff - 18.02.2017
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
#include "SerialBuffer.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
SerialBuffer::SerialBuffer()
{
	Clear(); // initialise the array in a correct way.
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void SerialBuffer::Clear()
{
	for (int i = 0; i < MAX_SERIALBUFFER_LENGTH; i++)
	{
		c[i] = 0x00;
	}

	currentPos = 0;
}

void SerialBuffer::Flush()
{
	Serial.write(C_MSGSTART);
	for (int i = 0; i < MAX_SERIALBUFFER_LENGTH; i++)
	{
		if (c[i] != 0x00)
		{
			Serial.write(c[i]);
			c[i] = 0x00;
		}
		else
		{
			Serial.write(C_MSGEND);
			Serial.println("");
			i = MAX_SERIALBUFFER_LENGTH;
		}
	}

	currentPos = 0;
}

void SerialBuffer::AddMsg(uint8_t Code, String Msg)
{
	if (currentPos + Msg.length() + 2 >= MAX_SERIALBUFFER_LENGTH)   // es soll kein Overflow erzeugt werden.
	{
		#if defined(autoFlush) && (autoFlush) == 1
			Flush();
		#else
			Clear();
		#endif
	}

	c[currentPos] = Code;
	currentPos++;

	for (int i = 0; i < Msg.length(); i++)
	{
		c[currentPos] = Msg.charAt(i);
		currentPos++;
	}

	c[currentPos] = C_MSGSEPARATOR;
	currentPos++;
}

void SerialBuffer::AddMsg(uint8_t Code, bool b)
{
	if (b)
	{
		AddMsg(Code, String("True"));
	}
	else
	{
		AddMsg(Code, String("False"));
	}
	//AddMsg(Code, String(b));
}

void SerialBuffer::AddMsg(uint8_t Code, int i)
{
	AddMsg(Code, String(i));
}

void SerialBuffer::AddMsg(uint8_t Code, double d)
{
	AddMsg(Code, String(d, 6));   // hier kann die standard-genauigkait der übertragenen double Werte eingestellt werden.
}
void SerialBuffer::AddMsg(uint8_t Code, double d, uint8_t Genauigkeit)
{
	AddMsg(Code, String(d, Genauigkeit));
}
