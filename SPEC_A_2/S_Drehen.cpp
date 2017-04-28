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
	status = Running;  // todo move this to Parent
	
	startTime = millis();
	counter = 0;
	
	MotorSpeedL = 0;
	MotorSpeedR = 0;

	finishedCounter = 0;
	stuckCounter = 0;
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

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_DrehenClass::Think()
{
	counter++;

	/*Serial.print("Status: ");
	Serial.println(status);*/

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
		//status = Error;
		status = Finished;  // There is no errorhandler so continue without. //todo change this.
		MotorSpeedL = 0;
		MotorSpeedR = 0;

		Serial.println("FAILURE: Drehen konnte nicht beendet werden. MaxTimer erreicht.");
	}

	if (ZielRichtung < 5 && ZielRichtung > 0)
	{
		float winkelAbstand = spectator->mpuFahrer.GetWinkelAbstand(ZielRichtung, spectator->mpu.GetYaw());
		
		if (stuckCounter > 0)
		{
			stuckCounter--;
		}
		if (abs(letzterWinkelAbstand - winkelAbstand) < 0.01)  // todo adapt this value
		{
			stuckCounter += 2;   // erhöhen obwol er oben verringert wurde.
		}
		else
		{
			stuckCounter = 0;  // StuckCounter gänzlich zurücksetzen.
		}

		if (abs(winkelAbstand) <= 0.01)  // stopp-toleranz   // 0.1 = 5,7°
		{
			finishedCounter++;
			MotorSpeedL = 0;
			MotorSpeedR = 0;
		}
		if (finishedCounter > 10)
		{
			this->status = Finished;

			Serial.println("S_Drehen.Think(): Drehen anscheinend beendet.");
		}
		else
		{
			int	motorspeed = (int)(winkelAbstand * S_Drehen_MotorSteigung);  // kein y-Offset da er sonst anfängt zu "schwingen"   // todo: insert a convenient function
			if (motorspeed > 0)
			{
				motorspeed += stuckCounter * S_Drehen_StuckSteigung;
			}
			else
			{
				motorspeed -= stuckCounter *S_Drehen_StuckSteigung;
			}

			// Motordrehrichtungen zuweisen.
			MotorSpeedL = motorspeed;
			MotorSpeedR = -motorspeed;

			// Die jeweils vorwärtsfahrende Kette muss ein bisschen beschleunigt werden damit sie sich gleichmäßig dreht.
			if (MotorSpeedL > 0)
			{
				MotorSpeedL += 10;
			}
			if (MotorSpeedR > 0)
			{
				MotorSpeedR += 10;
			}

			// Control MotorLimits.
			MotorSpeedL = spectator->Motoren.CapSpeed(MotorSpeedL, S_Drehen_MaxSpeed, S_Drehen_MinSpeed);
			MotorSpeedR = spectator->Motoren.CapSpeed(MotorSpeedR, S_Drehen_MaxSpeed, S_Drehen_MinSpeed);
		}

		letzterWinkelAbstand = winkelAbstand;
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
