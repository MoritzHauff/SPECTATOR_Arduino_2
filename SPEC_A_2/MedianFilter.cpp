/** MedianFilterClass.c
***
*** Die MedianFilter-Class verwaltet Messwerte
*** und wendet auf diese einen eindimensionalen MedianFilter an.
***
*** Moritz Hauff - 30.12.2016
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
#include "MedianFilter.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
MedianFilterClass::MedianFilterClass()
{
	this->ready = false;
	this->currentpos = 0;
	for (int i = 0; i < MAXARRAYLENGTH; i++)
	{
		this->measurements[i] = 0;   // korrekte Initialisierung des Arrays.
	}
}

void MedianFilterClass::Init(int TotalNumberOfValues)
{
	this->TotalNumberOfValues = TotalNumberOfValues;

	if (TotalNumberOfValues < MAXARRAYLENGTH)
	{
		this->ready = true;   // Alles wurde korrekt initialisiert.
	}
	else
	{
		// Fehler (ausgeben)
	}
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void MedianFilterClass::Update(int measurement)
{
	if (ready)
	{
		measurements[currentpos] = measurement;
		currentpos++;
		if (currentpos >= TotalNumberOfValues)
		{
			currentpos = 0;   // Keinen Speicher�berlauf generieren sondern wieder von vorne anfangen.
		}
	}
}

int MedianFilterClass::GetValue()
{
	if (ready)
	{
		for (int k = 1; k < TotalNumberOfValues; k++)   
		{
			for (int i = 0; i < TotalNumberOfValues - 1 - k; i++)
			{
				if (measurements[i] > measurements[i + 1])
				{
					double temp = measurements[i];
					measurements[i] = measurements[i + 1];
					measurements[i + 1] = temp;
				}
			}
		}

		// nachdem die Daten einmal ausgelesen wurden passt sind die Daten im Array nicht mehr zeitlich 
		// geordnet und sie sollten komplett neu aufgef�llt werden, damit es nicht zu Auswertungsfehlern kommt.
		currentpos = 0;
		
		/*Serial.print("MedianFilter: Verwendete ArrayPosition:");
		Serial.println(TotalNumberOfValues / 2);*/
		return measurements[TotalNumberOfValues / 2];
	}
	return 0;
}

void MedianFilterClass::PrintProps()
{
	Serial.print("TotalNumberOfValues: ");
	Serial.print(TotalNumberOfValues, 3);
}
