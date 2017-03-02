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
	char buffer[SERIALBUFF_SIZE + 1]; // +1 allows space for the null terminator
	int length = 0; // number of characters currently in the buffer

	while (Serial.available())
	{
		char c = Serial.read();
		if ((c == '\r') || (c == '\n'))  // Nachrichten müssen daher auch unbedingt mit einem Zeilenende versehen werden!
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

int Functions::ConvertCharToVorzeichen(char c)
{
	if (c == '+') 
	{
		return 1;
	}
	else if (c == '-')
	{
		return -1;
	}
	else
	{
		// todo: Fehler ausgeben.
		return 0;
	}
}

//Protokoll für manuelle MotorenSteuerung: C_TELEOPSTART +/- ValueLinks +/- ValueRechts C_TELEOPSTOP
void Functions::handleReceivedMessage(char *msg)
{
	/*//Debug
	Serial.println("Analysiere Daten");

	for (int i = 0; i < SERIALBUFF_SIZE; i++)
	{
		Serial.print((byte)msg[i]);
		Serial.print(" ");
	}*/
	
	if (msg[0] == C_TELEOPSTART)
	{
		if (msg[5] == C_TELEOPSTOP)
		{
			l = ConvertCharToVorzeichen(msg[1]) * (byte)msg[2];
			r = ConvertCharToVorzeichen(msg[3]) * (byte)msg[4];

			/*Serial.print("Neue Motordaten erhalten: l=");
			Serial.print(l);
			Serial.print(" r=");
			Serial.println(r);*/
		}
	}
}
