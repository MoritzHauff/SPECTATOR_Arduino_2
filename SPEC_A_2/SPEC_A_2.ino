/** SPEC_A_2.ino
***
*** Dieses Programm l�uft auf dem SPECTATOR-Arduino und �berwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek. Und angeschlossener SHARP-Distanzsensoren,
*** sowie des MPU6050s. ZeitTests der MLX-Temp-Sensoren und der Sto�sensoren Vorne.
***
*** Serielle �bertragung aller Sensorwerte und Steuerung der Motoren.
***
*** Moritz Hauff, 01.03.2017
**/

/// �bertragungsprotokoll:
/// Startsequenz -- 2xMotordaten -- RescueKits -- Endsequenz
/// A -- 2x(Linke/rechte Daten - Vorw�rts/R�ckw�rts - Geschwindigkeitswert(0-255)) - kein Kit/linkesKit/rechtesKit -- B
/// A -- 2x(L/R - +/- - 010) - 0/1/2/3/4 -- B
/// Bsp.: AL+124R-0451B

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECTATORClass.h"
#include "Constants.h"
 
///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *MotorL = AFMS.getMotor(1);		// Select which 'port' M1, M2, M3 or M4.
Adafruit_DCMotor *MotorR = AFMS.getMotor(2);
Adafruit_DCMotor *RescueMotoren = AFMS.getMotor(3);
Adafruit_DCMotor *UnterflurBeleuchtung = AFMS.getMotor(4);

float sensorValue;
bool ledState = false;

bool swtLinks = false;
bool swtRechts = false;

int r = 0;  // Motorspeed rechts
int l = 0;  // Motorspeed links

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	SA.Init();

	Serial.print("Starte MotorShield.");
	AFMS.begin();  // create with the default frequency 1.6KHz
	Kontrolllauf();
	Serial.print("MotorShield gestartet.");
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	
	SA.UpdateSharp();
	
	if (ledState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		SA.UpdateMLX();
	}
	
	SA.UpdateMPU();

	swtLinks = digitalRead2f(switchLinks_Pin);
	swtRechts = digitalRead2f(switchRechts_Pin);  // auch bei extremst kurzem Bet�tigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	SA.serialBuffer.AddMsg(C_SwitchLinks, swtLinks);
	SA.serialBuffer.AddMsg(C_SwitchRechts, swtRechts);

	SA.serialBuffer.Flush();

	zwei = micros();

	Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife f�hrt zu keinen Fifo-Overflows!
	Serial.println(" us."); 

	//functions.handleSerial();   // Sp�ter werden damit die ankommenden seriellen Nachrichten analysiert.
	leseSerial();
	SetMotoren(l, r);

	ledState = !ledState;
	digitalWrite2f(led_pin, ledState);   // Make the heartbeat.
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
bool SetMotorR(int Speed)
{
	if (Speed > 0 && Speed <= 255)
	{
		MotorR->run(FORWARD);
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorR->run(BACKWARD);
		Speed = -Speed;
	}
	else if (Speed == 0)
	{
		MotorR->run(RELEASE);
	}
	else
	{
		Serial.println("MotorR speed out of bounds");
		return false;
	}
	MotorR->setSpeed(Speed);
	return true;
}

bool SetMotorL(int Speed)
{
	if (Speed > 0 && Speed <= 255)
	{
		MotorL->run(FORWARD);
	}
	else if (Speed < 0 && Speed >= -255)
	{
		MotorL->run(BACKWARD);
		Speed = -Speed;
	}
	else if (Speed == 0)
	{
		MotorL->run(RELEASE);
	}
	else
	{
		Serial.println("MotorL speed out of bounds");
		return false;
	}
	MotorL->setSpeed(Speed);
	return true;
}

bool SetMotoren(int SpeedL, int SpeedR)
{
	if (SetMotorL(SpeedL) && SetMotorR(SpeedR))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Kontrolllauf(void)
{
	SetMotorR(-150);
	SetMotorL(-150);
	//delay(1000);
	delay(100);

	SetMotorR(0);
	SetMotorL(0);
	/*delay(1000);

	LED.switchOn();
	RescueKitAbwerfen();*/
}


int c_array_to_int(char Value[4])
{
	int returnvalue;

	//Serial.print("c_Array: ");
	//Serial.println(Value);

	returnvalue = 0;     // Beispielsumrechnung: Char zu int konvertieren, geschieht durch -48
						 //Serial1.println(returnvalue);  // Nur f�r Testzwecke

	for (int i = 1; i < 4; i++)   // Nur die Zahlen analysieren.
	{
		//Serial.print(Value[i]);
		if (Value[i] != ' ')    // Nochmal eine Kontrolle
		{
			returnvalue = returnvalue * 10;
			returnvalue = returnvalue + Value[i] - 48;
		}
		//Serial.println(returnvalue);    //Nur f�r Testzwecke
	}

	if (Value[0] == '-')
	{
		returnvalue = returnvalue * -1;
	}
	else if (Value[0] == '+')
	{
		// do nothing
	}
	else
	{
		//Serial.println("Fehler in der Daten�bertragung");
	}

	//Serial.println(returnvalue);        //Nur f�r Testzwecke
	return returnvalue;
}


void leseSerial()
{
	while (Serial.available() >= Length_of_SerialMessage)
	{
		char Message[Length_of_SerialMessage - 2];
		digitalWrite(13, HIGH);
		//Serial.print("Daten erhalten.");

		boolean started = false;
		boolean notfinished = true;
		int pos = 0;
		while (Serial.available() && notfinished)  // Seriellen Buffer lesen
		{
			char c = Serial.read();

			if (c == 'B')  // Ende erreicht
			{
				started = false;
				notfinished = false;
				//Serial.print("Ende der XBee-Message erreicht:"); Serial.println(i);
				//Serial.flush();  // restlichen Speicher leeren.
			}
			if (started)
			{
				Message[pos] = c;  // Tats�chliche Informationen in einen MessageBuffer kopieren.
								   //Serial.print(pos); Serial.print(":"); Serial.write(Message[pos]);
				pos++;
			}
			if (c == 'A')  // Anfangszeichen gesehen.
			{
				started = true;
				//Serial.print("Anfang der XBee-Message erreicht:"); Serial.println(i);
			}

		}

		char valueStellen[4] = { Message[1], Message[2], Message[3], Message[4] };
		int wert1 = c_array_to_int(valueStellen);

		char valueStellen2[4] = { Message[6], Message[7], Message[8], Message[9] };
		int wert2 = c_array_to_int(valueStellen2);

		if (Message[0] == 'R')
		{
			r = wert1;
		}
		if (Message[5] == 'R')
		{
			r = wert2;
		}
		if (Message[0] == 'L')
		{
			l = wert1;
		}
		if (Message[5] == 'L')
		{
			l = wert2;
		}

		int rescueKits = Message[10] - 48;  // Das letzte Nachrichtenzeichen gibt an ob ein RescueKit abgeworfen werden soll. (0 kein, 1 links, 2 rechts)

		if (rescueKits == 1)
		{
			//LinksDrop();
		}
		else if (rescueKits == 2)
		{
			//LinksLoad();
		}
		else if (rescueKits == 3)
		{
			//RechtsDrop();
		}
		else if (rescueKits == 4)
		{
			//RechtsLoad();
		}


		//////
		/*Serial.print("\tR:");     //Nur f�r Testzwecke
		Serial.println(r);
		Serial.print("\tL:");     //Nur f�r Testzwecke
		Serial.println(l); */
		//////


		digitalWrite(13, LOW);
	}
	Serial.flush();

}

