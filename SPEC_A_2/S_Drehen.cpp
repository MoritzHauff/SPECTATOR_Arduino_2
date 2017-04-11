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

	switch (status)
	{
	case Running:
		running();
		break;
	case Aborted:
	case Error:
	case Finished:
		MotorSpeedL = 0;
		MotorSpeedR = 0;
		break;
	default:
		MotorSpeedL = 0;
		MotorSpeedR = 0; 
		Serial.println("ERROR: S_Drehen.Think(): Fehler in der StateMachine.");
		break;
	}	
}

void S_DrehenClass::running()
{
	if (startTime + S_Drehen_Timer < millis() && status == Running)
	{
		status = Error;
		MotorSpeedL = 0;
		MotorSpeedR = 0;

		Serial.println("FAILURE: Drehen konnte nicht beendet werden. MaxTimer erreicht.");
	}

	if (ZielRichtung < 5 && ZielRichtung > 0)
	{
		if (spectator->mpuFahrer.BerechneDrehen(ZielRichtung, spectator->mpu.GetYaw(), &MotorSpeedL, &MotorSpeedR))  // Drehen beendet
		{
			status = Finished;

			Serial.println("S_Drehen.Think(): Drehen anscheinend beendet.");
		}
	}
	else
	{
		status = Error;
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
