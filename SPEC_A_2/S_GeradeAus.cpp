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
	// Variablen zurücksetzen.
	status = Running;

	stoppWahrscheinlichkeit = 0;
	winkelKorrektur = 0;
	encoderL = 0;
	encoderR = 0;

	speedL = S_GeradeAus_NormalSpeed * Direction;
	speedR = S_GeradeAus_NormalSpeed * Direction;

	// Himmelsrichtung ermitteln.
	startRichtung = spectator->mpuFahrer.CalculateRichtung(spectator->mpu.GetYaw());
	winkelKorrektur = spectator->mpuFahrer.BerechneVorwaerts(startRichtung, spectator->mpu.GetYaw());

	if (abs(winkelKorrektur) > 0.1)
	{
		status = Aborted;
		Serial.println("ABORTED S_GeradeAus.Init(): Spectator steht zu schief, bitte zuerst eine Drehung ausführen!");
	}

	// Wandkategorie ermitteln.
	laserFilter.Init(5);
	startWandKategorie = ermittleStartWandKategorie();
	zielWandKategorie = errechneZielWandKategorie(startWandKategorie, Direction);

	// todo bei langen strekcen wandeinlass mit sharp sensoren erkennen!

	// Debugausgaben.
	Serial.print("Fahre GeradeAus. Fahrtrichtung: ");
	Serial.println(Direction);
 
	Serial.print("WandKategorie: ");
	Serial.println(startWandKategorie);
	Serial.print(" ZielKategorie: ");
	Serial.print(zielWandKategorie);
	Serial.print(" Zielentfernung: ");
	Serial.println(S_GeradeAus_WandEntfernungen[zielWandKategorie]);

	// Timer starten.
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

	spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();    // Alle Sensornachrichten auf einmal sind zu lang für den SerialBuffer.

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateEncoder();

	spectator->serialBuffer.Flush();
	//spectator->serialBuffer.Clear();
}

bool S_GeradeAusClass::abweichungZuGros(int Value1, int Value2, int MaxAbweichung)
{
	if (Value1 + MaxAbweichung < Value2)
	{
		return true;
	}
	if (Value1 - MaxAbweichung > Value2)
	{
		return true;
	}

	return false;
}

void S_GeradeAusClass::Think()
{
	toggleState = !toggleState;

	// Winkelkorrektur ermitteln
	winkelKorrektur = spectator->mpuFahrer.BerechneVorwaerts(startRichtung, spectator->mpu.GetYaw());

	int h = winkelKorrektur * S_GeradeAus_WinkelRatio;

	/*Serial.print("momentan berechnete (MPU) Winkelkorrektur: ");
	Serial.println(winkelKorrektur);*/
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
	kontrolliereFortschritt();

	if (status == Running && startTime + S_GeradeAus_MaxTimer < millis())
	{
		speedL = 0;
		speedR = 0;
		status = Error;

		Serial.println("ERROR S_GeradeAus.Think(): Felddurchquerung hat zu lange gedauert! MaxTimer abgelaufen.");
	}
	
	if (status == Running && stoppWahrscheinlichkeit >= S_GeradeAus_MaxStoppWahrscheinlichkeit)
	{
		speedL = 0;
		speedR = 0;
		status = Finished;

		Serial.println("S_GeradeAus.Think(): Felddurchquerung beendet.");
	}
}

/*Kontrolliert den Fortschritt der Felddurchquerung und passt gegebenfalls die StoppWahrscheinlichkeit an.*/
void S_GeradeAusClass::kontrolliereFortschritt()
{
	if (stoppWahrscheinlichkeit >= 10)
	{
		stoppWahrscheinlichkeit -= 10;    // Jeden Tick verringert sich die Stoppwahrscheinlihckeit wieder falls es mal zu einem "Fehlalarm" gekommen ist.
	}
	else if (stoppWahrscheinlichkeit < 0)
	{
		stoppWahrscheinlichkeit = 0;  // War der letzte Tick noch dagegen zu stopnnen wird in jeden neuen Tick diese Entscheidung wieder neutral getroffen.
	}

	// Timer kontrollieren
	if (status == Running && startTime + S_GeradeAus_FeldTraversTimer < millis())
	{
		stoppWahrscheinlichkeit += 20;
	}

	// Bumper kontrollieren
	if (spectator->switchLinks.GetLastState() == true)
	{
		stoppWahrscheinlichkeit += 80;
		Serial.println("WARNING S_GeradeAus: linker Bumper hat Kontakt.");
	}
	if (spectator->switchRechts.GetLastState() == true)
	{
		stoppWahrscheinlichkeit += 80;
		Serial.println("WARNING S_GeradeAus: rechter Bumper hat Kontakt.");
	}

	// Encoder kontrollieren
	encoderL += spectator->Motoren.GetEncoderInfoL().CountsSinceLastTick;
	encoderR += spectator->Motoren.GetEncoderInfoR().CountsSinceLastTick;
	/*Serial.print("EncoderInfoL: ");
	Serial.print(encoderL);
	Serial.print(" R: ");
	Serial.println(encoderR);*/  // 30 cm = 16 EncoderCounts
	if ((abs(encoderL) < 8 || abs(encoderR) < 8) && stoppWahrscheinlichkeit > 0)
	{
		stoppWahrscheinlichkeit -= 40;  // Das Feld kann noch gar nicht durchquert sein.
	}
	else if ((abs(encoderL) < 16 || abs(encoderR) < 16) && stoppWahrscheinlichkeit > 0)
	{
		stoppWahrscheinlichkeit -= 20;
	}
	else if ((abs(encoderL) > 17 || abs(encoderR) > 17))
	{
		stoppWahrscheinlichkeit += 30;
	}

	/*Serial.print("LaserEntfernung: ");
	Serial.println(spectator->laserVorne.GetDistance());
	Serial.print("USEntfernung: ");
	Serial.println(spectator->ultraschallVorne.GetDistance());*/
	if (Direction == 1)
	{
		if (spectator->laserVorne.GetDistance() < S_GeradeAus_WandEntfernungenVor[zielWandKategorie])
		{
			stoppWahrscheinlichkeit += 50;

			Serial.print("S_GeradeAus.Think(): Laser empfiehlt die Felddurchquerung zu beenden. Entfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
		if (spectator->ultraschallVorne.GetDistance() <= S_GeradeAus_MinUltraschallEntfernung)
		{
			stoppWahrscheinlichkeit += 60;

			Serial.print("S_GeradeAus.Think(): Ultraschall detektiert vorne ein Hindernis. Entfernung: ");
			Serial.println(spectator->ultraschallVorne.GetDistance());
		}
	}
	if (Direction == -1)
	{
		if (spectator->laserVorne.GetDistance() > S_GeradeAus_WandEntfernungenRueck[zielWandKategorie])
		{
			stoppWahrscheinlichkeit += 50;

			Serial.print("S_GeradeAus.Think(): Laser empfiehlt die Felddurchquerung zu beenden. Entfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
		if (spectator->ultraschallHinten.GetDistance() <= S_GeradeAus_MinUltraschallEntfernung)
		{
			stoppWahrscheinlichkeit += 60;

			Serial.print("S_GeradeAus.Think(): Ultraschall detektiert hinten ein Hindernis. Entfernung: ");
			Serial.println(spectator->ultraschallVorne.GetDistance());
		}
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

/*Führt die nötigen Messungen durch um die aktuelle Wandkategorie zu ermitteln und gibt diese zurück.*/
byte S_GeradeAusClass::ermittleStartWandKategorie()
{
	int laserDistance;
	for (int i = 0; i < 5; )
	{
		Sense();
		if (spectator->laserVorne.NewDataAvaiable() == true)   // Unbedingt eine korrekte Laserentfernung ermitteln damit die richtige Wandkategorie verwendet wird.
		{
			i++;
			laserFilter.Update(spectator->laserVorne.GetDistance());
			Serial.print("neue Laserentfernung: ");
			Serial.println(spectator->laserVorne.GetDistance());
		}
	}
	laserDistance = laserFilter.GetValue();

	int usDistance = spectator->ultraschallVorne.GetDistance() * 10;  // die ultraschallentfernung angepasst an die einheit des lasers.
	if (abweichungZuGros(laserDistance, usDistance, 50))
	{
		Serial.println("WARNING: zu große Abweichung zwischen Ultraschall und Laser, korrekte Zielwandkategorie?");
		if (usDistance > laserDistance)
		{
			laserDistance += 50;
		}
		else if (usDistance < laserDistance)
		{
			laserDistance -= 50;
		}
	}

	Serial.print("S_GeradeAus.ermittleStartWandKategorie(): verwendete Laserentfernung: ");
	Serial.println(laserDistance);

	return entfernungZuWandKategorie(laserDistance, Direction);
}

/*Errechnet anhand der übergebenen StartWandKategorie die richtige ZielWandKategorie in der jeweiligen Fahrtrichtung.*/
byte S_GeradeAusClass::errechneZielWandKategorie(byte StartWandKategorie, int Direction)
{
	byte zielWandKategorie = startWandKategorie - Direction;   // todo: Achtung dass kann hier NULL werden!!!!

	if (zielWandKategorie == 255)
	{
		zielWandKategorie = 0;
	}
	if (zielWandKategorie >= sizeof(S_GeradeAus_WandEntfernungen))
	{
		zielWandKategorie = sizeof(S_GeradeAus_WandEntfernungen) - 1;  // todo wenn er beim vorherigen schritt zu kurz gefahrne ist sollte die richtige kategorie verwendet werden.
	}

	return zielWandKategorie;
}

/*Wandelt eine Entfernung in die enntsprechende Wandkategorie in der jeweiligen Entfenrung um.*/
byte S_GeradeAusClass::entfernungZuWandKategorie(int Distance, int Direction)
{
	byte b = 0;
	int minAbstand = abs(Distance - S_GeradeAus_WandEntfernungen[b]);
	
	for (byte i = 1; i < sizeof(S_GeradeAus_WandEntfernungen); i++)
	{
		int h = abs(Distance - S_GeradeAus_WandEntfernungen[i]);
		if (h < minAbstand)
		{
			minAbstand = h;
			b = i;
		}
	}

	return b;
}
