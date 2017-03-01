/** Functions.cpp
***
*** Alle übrig gebliebenen Funktionen, die in keine andere Datei gepasst
*** haben. 
***
*** Moritz Hauff, 17.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "Functions.h"

///////////////////////////////////////////////////////////////////////////
///Instanz
Functions functions;

///////////////////////////////////////////////////////////////////////////
///Functions
void Functions::handleSerial()
{
	static char buffer[SERIALBUFF_SIZE + 1]; // +1 allows space for the null terminator
	static int length = 0; // number of characters currently in the buffer

	while (Serial.available())
	{
		char c = Serial.read();
		if ((c == '\r') || (c == '\n'))
		{
			// end-of-line received
			if (length > 0)
			{
				handleReceivedMessage(buffer);
			}
			length = 0;
		}
		else
		{
			if (length < SERIALBUFF_SIZE)
			{
				buffer[length++] = c; // append the received character to the array
				buffer[length] = 0; // append the null terminator
			}
			else
			{
				// buffer full - discard the received character
			}
		}
	}
}

void Functions::handleReceivedMessage(char *msg)
{
	//...
}