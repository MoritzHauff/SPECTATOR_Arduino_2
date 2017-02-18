/** SerialBuffer.cpp
***
*** Die SerialBuffer-Klasse verpackt Sensordaten in ein Übertragungsformat
*** und schickt sie über den seriellen Port an den RaspberryPi.
***
*** Moritz Hauff - 18.02.2017
**/

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
	if (currentPos + Msg.length() >= MAX_SERIALBUFFER_LENGTH)   // es soll kein Overflow erzeugt werden.
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

void SerialBuffer::AddMsg(uint8_t Code, int i)
{
	AddMsg(Code, String(i));
}

/*void SerialBuffer::Send(uint8_t Code, int i)
{
	Serial.write(C_MSGSTART);
	Serial.write(Code);
	Serial.println(i);
	//Serial.println(C_MSGEND);
}

void SerialBuffer::Send(uint8_t Code, double d)
{
	Serial.print(C_MSGSTART);
	Serial.print(Code);
	Serial.print(d);
	Serial.println(C_MSGEND);
}*/

