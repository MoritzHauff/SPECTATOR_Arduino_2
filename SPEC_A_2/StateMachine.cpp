/** StateMachine.cpp
***
*** Diese Klasse überwacht die aktuellen Aufgaben des Roboters.
*** Analysiert außerdem die seriellen Befehle.
***
*** Moritz Hauff, 17.03.2017
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
#include "StateMachine.h"

StateMachineClass::StateMachineClass(SPECTATORClass *Spectator)
{
	spectator = Spectator;
	//curentState = Idle;

	s_Fahren = new S_FahrenClass(spectator, "Fahren");
	
	changeState(s_Fahren);
}

StateMachineClass::~StateMachineClass()
{
	// clean-up
	delete currentState;
	delete s_Fahren;
}

///////////////////////////////////////////////////////////////////////////
///Functions
void StateMachineClass::handleSerial()
{
	char buffer[SERIALBUFF_SIZE + 1]; // +1 allows space for the null terminator
	int pos = 0; // number of characters currently in the buffer

	while (Serial.available())
	{
		char c = Serial.read();

		//Serial.print((byte)c);
		//Serial.print(" ");

		if ((c == '\r') || (c == '\n'))  // Nachrichten müssen daher auch unbedingt mit einem Zeilenende versehen werden!
		{
			// end-of-line received
			/*Serial.print("length=");
			Serial.print(length);    // manchmal ist beim ende der nachricht die länge falsch (=0)
			if (length > 0)
			{
			Serial.println(" >>");
			handleReceivedMessage(buffer);
			}
			length = 0;*/

			handleReceivedMessage(buffer);
			pos = 0;

			//Serial.println(" >>");
		}
		else
		{
			if (pos < SERIALBUFF_SIZE)
			{
				buffer[pos++] = c; // append the received character to the array
				buffer[pos] = 0; // append the null terminator
			}
			else
			{
				// buffer full - discard the received character
			}
		}
	}

	// if there was no newline analyse you could analyse it her anyway
	//handleReceivedMessage(buffer);
}

int StateMachineClass::convertCharToVorzeichen(char c)
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
void StateMachineClass::handleReceivedMessage(char *msg)
{
	//Debug
	//Serial.println("Analysiere Daten");

	/*for (int i = 0; i < SERIALBUFF_SIZE; i++)
	{
	Serial.print((byte)msg[i]);
	Serial.print(" ");
	}
	Serial.println(" >>");*/

	if (msg[0] == C_TELEOPSTART)
	{
		if (msg[5] == C_TELEOPSTOP)
		{
			spectator->MotorSpeedL = convertCharToVorzeichen(msg[1]) * (byte)msg[2];
			spectator->MotorSpeedR = convertCharToVorzeichen(msg[3]) * (byte)msg[4];

			/*Serial.print("Neue Motordaten erhalten: l=");
			Serial.print(l);
			Serial.print(" r=");
			Serial.println(r);*/
			
			// todo: Modus entscheiden.
			changeState(s_Fahren);
		}
		if (msg[3] == C_TELEOPSTOP)
		{
			if (msg[1] == 'd')
			{
				// todo: Modus eintragen.
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
///State-Functions
void StateMachineClass::changeState(StateClass *NextState)
{
	if (currentState != NextState)
	{
		currentState = NextState;
		
		if (currentState == s_Fahren)
		{
			s_Fahren->Init();  // mit currentState werden die Variablen nicht korrekt initialisiert.
		}

		Serial.println(String("Neuer Modus: " + currentState->GetName()));  // for debugging on the RaPi
	}
}

void StateMachineClass::DoAction()
{
	handleSerial();

	if (currentState == s_Fahren) 
	{
		/*currentState->Sense();
		currentState->Think();
		currentState->Act();*/
		s_Fahren->Sense();   // wird über die gecastete Instanz currentState zugegriffen werden Variablen die innerhalb des States definiert und verwendet werden (wie z.B. Toggle) nicht korrekt angesprochen.
		s_Fahren->Think();
		s_Fahren->Act();
	}

	
}