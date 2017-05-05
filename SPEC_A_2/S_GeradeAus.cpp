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
	straightCounter = 0;
	turnCounter = 0;

	speedL = S_GeradeAus_NormalSpeed * Direction;
	speedR = S_GeradeAus_NormalSpeed * Direction;

	// Himmelsrichtung ermitteln.
	startRichtung = spectator->mpuFahrer.CalculateRichtung(spectator->mpu.GetYaw());
	winkelKorrektur = spectator->mpuFahrer.GetWinkelAbstand(startRichtung, spectator->mpu.GetYaw());

	/*if (abs(winkelKorrektur) > 0.1)
	{
		status = Aborted;
		Serial.println("ABORTED S_GeradeAus.Init(): Spectator steht zu schief, bitte zuerst eine Drehung ausführen!");
	}*/   // Es gibt keine Korrekturmaßnahmen also sollte dies nicht überprüft werden.

	// Wandkategorie ermitteln.
	laserFilter.Init(5);
	startWandKategorie = ermittleStartWandKategorie();
	zielWandKategorie = errechneZielWandKategorie(startWandKategorie, Direction);
	//aktZielEntfernung = 

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

	//spectator->OpferKontroller.Reset();
	 
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
		spectator->OpferKontroller.Check(spectator->MLXLinks.GetObjTemp(), spectator->MLXVorne.GetObjTemp(), spectator->MLXRechts.GetObjTemp());
	}

	spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateSwitches();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();    // Alle Sensornachrichten auf einmal sind zu lang für den SerialBuffer.

	spectator->UpdateLDR();

	spectator->UpdateHCSr04VorneHinten();

	spectator->UpdateEncoder();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
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

	if (turnCounter > 0)
	{
		turnCounter--;
		// The Motorspeed was set before.
		Serial.println("S_GeradeAus.Think(): Verringere turnCounter.");
	}
	else if (straightCounter > 0)
	{
		straightCounter--;
		adaptedSpeedL = S_GeradeAus_NormalSpeed * Direction;
		adaptedSpeedR = S_GeradeAus_NormalSpeed * Direction;
		Serial.println("S_GeradeAus.Think(): Verringere straightCounter.");
	}
	else
	{

		// Winkelkorrektur ermitteln
		winkelKorrektur = spectator->mpuFahrer.GetWinkelAbstand(startRichtung, spectator->mpu.GetYaw());

		winkelKorrektur += -verwerteSharp(spectator->sharplinksvorne.GetValue(), spectator->sharplinkshinten.GetValue());
		winkelKorrektur += verwerteSharp(spectator->sharprechtsvorne.GetValue(), spectator->sharprechtshinten.GetValue());

		/*Serial.print("momentan berechnete (MPU) Winkelkorrektur: ");
		Serial.println(winkelKorrektur);*/

		/*adaptedSpeedL = capSpeed(speedL + h * Direction, S_GeradeAus_NormalSpeed, 70);
		adaptedSpeedR = capSpeed(speedR - h * Direction, S_GeradeAus_NormalSpeed, 70);*/

		if (winkelKorrektur > 0.01)
		{
			adaptedSpeedL = 250 * Direction;
			adaptedSpeedR = 170 * Direction;   // 180
		}
		else if (winkelKorrektur < -0.01)
		{
			adaptedSpeedL = 170 * Direction;   // 180
			adaptedSpeedR = 250 * Direction;
		}
		else
		{
			adaptedSpeedL = S_GeradeAus_NormalSpeed * Direction;
			adaptedSpeedR = S_GeradeAus_NormalSpeed * Direction;
		}
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

/*Gibt eine Winkelkorrektur an. Pos: zur Wand hin, neg von der jeweiligen Wand weg.*/
float S_GeradeAusClass::verwerteSharp(int Vorne, int Hinten)
{
	if (Vorne < 150 || Hinten < 150)  // zu weit weg von den Wänden, keine Korrektur möglich
	{
		return 0;
	}
	else if (Vorne < 280 || Hinten < 280)  // zu weit aber in korrigierabstand.
	{
		return 0.1;
	}
	else if (Vorne > 640|| Hinten > 640)  // zu nah an der Wand drehe davon weg.
	{
		turnCounter = 5;
		straightCounter = 5;
		//Serial.println("S_GeradeAus.Think(): Sharp melden deutliche Naehe zur Wand, korrigiere...");
		return -0.15;
	}
	else if (Vorne > 420 || Hinten > 420)   // zu nah aber noch nicht so nah das gerade aus gefahren werden muss.
	{
		//Serial.println("S_GeradeAus.Think(): Sharp melden etwas Naehe zur Wand, korrigiere...");
		return -0.07;
	}
	else
	{
		int differenz = Hinten - Vorne;  // positiv wenn hinten näher an wand als vorne.
		
		if (abs(differenz) < 25)
		{
			return 0;
		}
		else if (differenz > 0)
		{
			//Serial.println("S_GeradeAus.Think(): Sharp verlangen richtungskorrektur.");
			return 0.05;
		}
		else if (differenz < 0)
		{
			//Serial.println("S_GeradeAus.Think(): Sharp verlangen richtungskorrektur.");
			return -0.05;
		}

		return 0;
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
	if ((abs(encoderL) < 10 || abs(encoderR) < 10) && stoppWahrscheinlichkeit > 0)
	{
		stoppWahrscheinlichkeit -= 60;  // Das Feld kann noch gar nicht durchquert sein.
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
			Serial.print(spectator->laserVorne.GetDistance());
			Serial.print(" Zielkategorie: ");
			Serial.print(zielWandKategorie);
			Serial.print(" Ziel entfernung :");
			Serial.println(S_GeradeAus_WandEntfernungenVor[zielWandKategorie]);
		}
		if (spectator->laserVorne.GetDistance() > S_GeradeAus_WandEntfernungenVor[zielWandKategorie] + 35 && spectator->laserVorne.GetDistance() < S_GeradeAus_WandEntfernungenVor[zielWandKategorie] + 120)  // noch deutlich zu weit weg von der wand.
		{
			if (stoppWahrscheinlichkeit >= 25)
			{
				stoppWahrscheinlichkeit -= 25;

				Serial.println("S_GeradeAus.Think(): Laser sehen bald das Feldende, muss noch ein bisschen weiterfahren.");
			}
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
