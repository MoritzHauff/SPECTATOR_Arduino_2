/** MedianFilterClass.c
***
*** Die MedianFilter-Class verwaltet Messwerte
*** und wendet auf diese einen eindimensionalen MedianFilter an.
***
*** Moritz Hauff - 30.12.2016
**/

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
			currentpos = 0;   // Keinen Speicherüberlauf generieren sondern wieder von vorne anfangen.
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
		// geordnet und sie sollten komplett neu aufgefüllt werden, damit es nicht zu Auswertungsfehlern kommt.
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
