/** S_TeleOp.cpp
***
*** Dieser State ermöglicht dem seriellen Operator die aktuelle Aktion zu 
*** unterbrechen und sie nach einem weiteren Befehl wieder aufzunehmen.
*** Dies soll dem Debugging dienen.
***
*** Moritz Hauff, 18.03.2017
**/

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
