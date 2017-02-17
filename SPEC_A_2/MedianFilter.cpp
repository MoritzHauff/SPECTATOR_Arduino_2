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
	for (int i = 0; i < ARRAYLENGTH; i++)
	{
		this->measurements[i] = 0;   // korrekte Initialisierung des Arrays.
		this->sortedMeasurements[i] = 0;
	}
}

void MedianFilterClass::Init(int TotalNumberOfValues)
{
	this->TotalNumberOfValues = TotalNumberOfValues;

	if (TotalNumberOfValues < ARRAYLENGTH)
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
		// aktuelle Daten sortieren.
		/*for (int i = 0; i < ARRAYLENGTH; i++)   // array kopereien und dann erst sorteiren dauert 300 us!
		{
			sortedMeasurements[i] = measurements[i];
		}*/

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
		
		return measurements[TotalNumberOfValues / 2];
	}
	return 0;
}

int MedianFilterClass::GetValue_Int()
{
	return (int)GetValue();
}

void MedianFilterClass::PrintProps()
{
	Serial.print("TotalNumberOfValues: ");
	Serial.print(TotalNumberOfValues, 3);
	//Serial.print("UsedValues: ");
	//Serial.print(NumberOfUsedValues, 3);
	//Serial.print("Dropped Values: ");
	//Serial.print(TotalNumberOfValues - NumberOfUsedValues);
}
