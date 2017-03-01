/** Functions.cpp
***
*** Alle übrig gebliebenen Funktionen, die in keine andere Datei gepasst
*** haben. 
***
*** Moritz Hauff, 17.02.2017
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