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
		float winkelAbstand = spectator->mpuFahrer.GetWinkelAbstand(ZielRichtung, spectator->mpu.GetYaw());
		
		if (abs(winkelAbstand) <= 0.01)  // stopp-toleranz   // 0.1 = 5,7°
		{
			status = Finished;

			Serial.println("S_Drehen.Think(): Drehen anscheinend beendet.");
		}
		else
		{
			int motorspeed = (int)(winkelAbstand * S_Drehen_MotorSteigung) + S_Drehen_MinSpeed;   // todo: insert a convenient function   // 360

			if (motorspeed > 0)
			{
				motorspeed = min(motorspeed, S_Drehen_MaxSpeed); // cap at 180
				motorspeed = max(motorspeed, S_Drehen_MinSpeed);  // below 60 the motors wont turn.
			}
			if (motorspeed < 0)
			{
				motorspeed = max(motorspeed, -S_Drehen_MaxSpeed); // cap at 180
				motorspeed = min(motorspeed, -S_Drehen_MinSpeed);  // below 60 the motors wont turn.   // todo cap mit der zeit erhöhen wenn er sich nicht mehr dreht.
			}

			MotorSpeedL = motorspeed;
			MotorSpeedR = -motorspeed;
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
