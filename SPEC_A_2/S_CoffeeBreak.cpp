/** S_TeleOp.cpp
***
*** Dieser State ermöglicht dem seriellen Operator die aktuelle Aktion zu 
*** unterbrechen und sie nach einem weiteren Befehl wieder aufzunehmen.
*** Dies soll dem Debugging dienen.
***
*** Moritz Hauff, 18.03.2017
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
#include "S_CoffeeBreak.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_CoffeeBreakClass::Init()
{
	lastToggle = millis();
	coffeBreakStart = millis();

	Serial.println("Befinde mich in der Kaffeepause! Type CONTINUE to continue.");
}

///////////////////////////////////////////////////////////////////////////
///Functions
unsigned long S_CoffeeBreakClass::GetTimerShiftAmount()
{
	return millis() - coffeBreakStart;
}

void S_CoffeeBreakClass::Sense()
{
	spectator->UpdateLaser();
	
	spectator->UpdateMPU();   // Lese den MPU6050 aus, damit es zu keinem FIO kommt.

	spectator->serialBuffer.Clear();	// Die Daten müssen aber nicht übermittelt werden, 
										// da wir uns in der KaffeePause befinden.
}

void S_CoffeeBreakClass::Think()
{
	// do nothing
}

void S_CoffeeBreakClass::Act()
{
	// stop the motors
	spectator->Motoren.SetMotoren(0, 0);

	// stelle die Kaffeepause mit einer blinkenden Unterflurbeleuchtung dar.
	if (lastToggle + BlinkDelay + BlinkDelay < millis())
	{
		spectator->Motoren.TurnLEDOff();
		lastToggle = millis();
	}
	else if (lastToggle + BlinkDelay < millis())
	{
		spectator->Motoren.TurnLEDOn();
	}
}
