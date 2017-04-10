// 
// todo
// 

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_Drehen.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
void S_DrehenClass::Init()
{
	status = Running;
	
	startTime = millis();   // todo: stop when the timer is over
	counter = 0;
	
	MotorSpeedL = 0;
	MotorSpeedR = 0;

}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_DrehenClass::Sense()
{
	spectator->UpdateSharp();

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	if (counter >= 5)
	{
		spectator->UpdateMLX();
		counter = 1;
	}

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();
}

void S_DrehenClass::Think()
{
	counter++;

	if (startTime + S_Drehen_Timer < millis() && ZielRichtung != 5 && ZielRichtung != 6)
	{
		ZielRichtung = 6;
		MotorSpeedL = 0;
		MotorSpeedR = 0;

		Serial.println("FAILURE: Drehen konnte nicht beendet werden. MaxTimer erreicht.");
	}
	
	if (ZielRichtung < 5 && ZielRichtung > 0)
	{
		if (spectator->mpuFahrer.BerechneDrehen(ZielRichtung, spectator->mpu.GetYaw(), &MotorSpeedL, &MotorSpeedR))  // Drehen beendet
		{
			ZielRichtung = 5;
		}
	}
	else if(ZielRichtung == 5)
	{
		MotorSpeedL = 0;
		MotorSpeedR = 0;
		
		Serial.println("S_Drehen.Think(): Drehen anscheinend beendet.");
		ZielRichtung = 6;
	}
	else if (ZielRichtung != 6)
	{
		status = Error;
		ZielRichtung = 6;
		Serial.println("ERROR: S_Drehen.Think(): Falsche Drehrichtung angegeben.");
	}
}

void S_DrehenClass::Act()
{
	spectator->Motoren.SetMotoren(MotorSpeedL, MotorSpeedR);
}

void S_DrehenClass::ShiftTimers(unsigned long ShiftAmount)
{
	/*Serial.print("StartZeit angepasst um [ms]:  "); // debug
	Serial.println(ShiftAmount);*/
	startTime += ShiftAmount;
}
