/** S_GeradeAus.cpp
***
*** Dieser State überwacht das GeradeAusfahren.
***
*** Moritz Hauff, 06.04.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_GeradeAus.h"

void S_GeradeAusClass::Init()
{
	status = Running;

	stoppWahrscheinlichkeit = 0;
	winkelKorrektur = 0;
	encoderL = 0;
	encoderR = 0;

	speedL = S_GeradeAus_NormalSpeed * Direction;
	speedR = S_GeradeAus_NormalSpeed * Direction;

	toggleState = false;  // Am anfang soll der Laser befragt werden.

	Sense();
	Sense();
	Sense();


	startRichtung = spectator->mpuFahrer.CalculateRichtung(spectator->mpu.GetYaw());
	winkelKorrektur = spectator->mpuFahrer.BerechneVorwaerts(startRichtung, spectator->mpu.GetYaw());

	if (abs(winkelKorrektur) > 0.1)
	{
		status = Aborted;
		Serial.println("S_GeradeAus.Init(): Spectator steht zu schief, bitte zuerst eine Drehung ausführen!");
	}

	startDistanceLaserV = spectator->laserVorne.GetDistance();
	startDistanceUSV = spectator->ultraschallVorne.GetDistance();
	startDistanceUSH = spectator->ultraschallHinten.GetDistance();

	startWandKategorie = ermittleStartWandKategorie(startDistanceLaserV);
	zielWandKategorie = startWandKategorie - Direction;   // todo: Achtung dass kann hier NULL werden!!!!

	Serial.print("Fahre GeradeAus. Fahrtrichtung: ");
	Serial.println(Direction);

	Serial.print("LaserDistanz: ");
	Serial.print(startDistanceLaserV); 
	Serial.print(" Kategorie: ");
	Serial.println(startWandKategorie);
	Serial.print("ZielKategorie: ");
	Serial.print(zielWandKategorie);
	Serial.print(" Zielentfernung: ");
	Serial.print(S_GeradeAus_WandErntfernungen[zielWandKategorie]);
	Serial.print("Ultraschall Distanz Vorne: ");
	Serial.println(startDistanceUSV);
	Serial.print("Ultraschall Distanz Hinten: ");
	Serial.println(startDistanceUSH);
	Serial.print("Speed: ");
	Serial.print(speedL); Serial.print(" ");
	Serial.println(speedR);

	startTime = millis();
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_GeradeAusClass::Sense()
{
	spectator->UpdateSharp();

	if (toggleState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		spectator->UpdateMLX();
	}
	else
	{
		//spectator->UpdateLaser();
	}

	spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();    // Alle Sensornachrichten auf einmal sind zu lang für den SerialBuffer.

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_GeradeAusClass::Think()
{
	toggleState = !toggleState;
	if (stoppWahrscheinlichkeit >= 10)
	{
		stoppWahrscheinlichkeit -= 10;    // Jeden Tick verringert sich die Stoppwahrscheinlihckeit wieder falls es mal zu einem "Fehlalarm" gekommen ist.
	}
	else if (stoppWahrscheinlichkeit < 0)
	{
		stoppWahrscheinlichkeit = 0;  // War der letzte Tick noch dagegen zu stopnnen wird in jeden neuen Tick diese Entscheidung wieder neutral getroffen.
	}

	// Winkelkorrektur ermitteln
	winkelKorrektur = spectator->mpuFahrer.BerechneVorwaerts(startRichtung, spectator->mpu.GetYaw());

	int h = winkelKorrektur * S_GeradeAus_WinkelRatio;

	Serial.print("momentan berechnete (MPU) Winkelkorrektur: ");
	Serial.println(winkelKorrektur);
	/*Serial.print("berechnete Korrekturmasnahme: ");
	Serial.print(h);*/

	/*adaptedSpeedL = capSpeed(speedL + h * Direction, S_GeradeAus_NormalSpeed, 70);
	adaptedSpeedR = capSpeed(speedR - h * Direction, S_GeradeAus_NormalSpeed, 70);*/

	if (winkelKorrektur > 0.01)
	{
		adaptedSpeedL = 250 * Direction;
		adaptedSpeedR = 180 * Direction;
	}
	else if (winkelKorrektur < -0.01)
	{
		adaptedSpeedL = 180 * Direction;
		adaptedSpeedR = 250 * Direction;
	}
	else
	{
		adaptedSpeedL = S_GeradeAus_NormalSpeed * Direction;
		adaptedSpeedR = S_GeradeAus_NormalSpeed * Direction;
	}

	//Stopp zeitpunkt ermitteln
	if (status == Running && startTime + S_GeradeAus_MaxTimer < millis())
	{
		speedL = 0;
		speedR = 0;
		status = Error;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung hat zu lange gedauert! MaxTimer abgelaufen.");
	}
	if (status == Running && startTime + S_GeradeAus_FeldTraversTimer < millis())
	{
		stoppWahrscheinlichkeit += 20;
	}

	encoderL += spectator->Motoren.GetEncoderInfoL().CountsSinceLastTick;
	encoderR += spectator->Motoren.GetEncoderInfoR().CountsSinceLastTick;
	Serial.print("EncoderInfoL: ");
	Serial.print(encoderL);
	Serial.print(" R: ");
	Serial.println(encoderR);  // 30 cm = 16 EncoderCounts

	Serial.print("LaserEntfernung: ");
	Serial.println(spectator->laserVorne.GetDistance());
	if (Direction == 1)
	{
		if (spectator->laserVorne.GetDistance() < S_GeradeAus_WandErntfernungen[zielWandKategorie] + S_GeradeAus_WandEntfernungsKorrektur)
		{
			stoppWahrscheinlichkeit += 50;

			Serial.print("S_GeradeAus.Think(): Laser empfiehlt die Felddurchquerung zu beenden. Entfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
	}
	if (Direction == -1)
	{
		if (spectator->laserVorne.GetDistance() > S_GeradeAus_WandErntfernungen[zielWandKategorie] - S_GeradeAus_WandEntfernungsKorrektur)
		{
			stoppWahrscheinlichkeit += 50;

			Serial.print("S_GeradeAus.Think(): Laser empfiehlt die Felddurchquerung zu beenden. Entfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
	}
	if ((abs(encoderL) < 10 || abs(encoderR) < 10) && stoppWahrscheinlichkeit > 0)
	{
		stoppWahrscheinlichkeit -= 40;  // Das Feld kann noch gar nicht durchquert sein.
	}
	else if ((abs(encoderL) < 16 || abs(encoderR) < 16) && stoppWahrscheinlichkeit > 0)
	{
		stoppWahrscheinlichkeit -= 20;
	}
	else if ((abs(encoderL) > 17 || abs(encoderR) < 17))
	{
		stoppWahrscheinlichkeit += 30;
	}

	if (status == Running && stoppWahrscheinlichkeit >= S_GeradeAus_MaxStoppWahrscheinlichkeit)
	{
		speedL = 0;
		speedR = 0;
		status = Finished;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung beendet.");
	}
}

void S_GeradeAusClass::Act()
{
	//spectator->Motoren.SetMotoren(adaptedSpeedL, adaptedSpeedR);
	
	if (Direction == 1)
	{
		spectator->Motoren.SetMotoren(adaptedSpeedL, adaptedSpeedR);
	}
	if (Direction == -1)
	{
		spectator->Motoren.SetMotoren(adaptedSpeedR, adaptedSpeedL);   // Richtung umkehren, somit muss nicht bei jeder Veränderung der Geschwindigkeit einzeln unterschieden werden.
	}

	spectator->HeartbeatLED.Toggle();
}

void S_GeradeAusClass::ShiftTimers(unsigned long ShiftAmount)
{
	startTime += ShiftAmount;
}

int S_GeradeAusClass::capSpeed(int Value, int Upper, int Lower)
{
	if (Value > 0)
	{
		Value = min(Value, Upper);
		Value = max(Value, Lower);
	}
	else
	{
		Value = min(Value, -Lower);
		Value = max(Value, -Upper);
	}

	return Value;
}

byte S_GeradeAusClass::ermittleStartWandKategorie(int StartDistance)
{
	byte b = 0;
	int minAbstand = abs(StartDistance - S_GeradeAus_WandErntfernungen[b]);
	
	for (byte i = 1; i < sizeof(S_GeradeAus_WandErntfernungen); i++)
	{
		int h = abs(StartDistance - S_GeradeAus_WandErntfernungen[i]);
		if (h < minAbstand)
		{
			minAbstand = h;
			b = i;
		}
	}

	return b;
}
