/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek. Und angeschlossener SHARP-Distanzsensoren,
*** sowie des MPU6050s. ZeitTests der MLX-Temp-Sensoren und der Stoßsensoren Vorne.
***
*** Serielle Übertragung aller Sensorwerte.
***
*** Moritz Hauff, 18.02.2017
**/

/// Übertragungsprotokoll:
/// Startsequenz -- 2xMotordaten -- RescueKits -- Endsequenz
/// A -- 2x(Linke/rechte Daten - Vorwärts/Rückwärts - Geschwindigkeitswert(0-255)) - kein Kit/linkesKit/rechtesKit -- B
/// A -- 2x(L/R - +/- - 010) - 0/1/2/3/4 -- B
/// Bsp.: AL+124R-0451B

///////////////////////////////////////////////////////////////////////////
///Includes
#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions

#include <Adafruit_MotorShield.h>

#include "SharpIR.h"
#include "MPU.h"
#include "MLX90614Class.h"

#include "SerialBuffer.h"

#include "Functions.h"
 

///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur für ihren im Setup angegeben Zweck nutzen, sonst kann es zur Beschädigung des ATmegas2560 kommen!
const GPIO_pin_t switchLinks_Pin = DP23;
const GPIO_pin_t switchRechts_Pin = DP25;

const uint8_t analog_Pin = A1;

const uint8_t DELAY_TIME = 100;

const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen würden damit 4 ms dauern.

const int Length_of_SerialMessage = 13;
const int length_of_information = 5;

///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

SharpIR sharplinksvorne(analog_Pin, SHARPMEASUREMTS);
SharpIR sharprechtsvorne(A2, SHARPMEASUREMTS);
SharpIR sharplinkshinten(A3, SHARPMEASUREMTS);
SharpIR sharprechtshinten(A4, SHARPMEASUREMTS);

MPU mpu = MPU();

MLX90614Class MLXLinks = MLX90614Class(0x2C);
MLX90614Class MLXVorne = MLX90614Class(0x2A);
MLX90614Class MLXRechts = MLX90614Class(0x2B);

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *MotorL = AFMS.getMotor(1);		// Select which 'port' M1, M2, M3 or M4.
Adafruit_DCMotor *MotorR = AFMS.getMotor(2);
Adafruit_DCMotor *RescueMotoren = AFMS.getMotor(3);
Adafruit_DCMotor *UnterflurBeleuchtung = AFMS.getMotor(4);

SerialBuffer serialBuffer = SerialBuffer();

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
	pinMode2f(led_pin, OUTPUT);
	pinMode2f(switchLinks_Pin, INPUT);
	pinMode2f(switchRechts_Pin, INPUT);
	
	Serial.begin(250000);  // Je höher die Baudrate und je mehr Daten im Serial.print stehen desto mehr Zeit wird gespart.
	Serial.println("SPEC_A_2 - Serial Start");

	// join I2C bus (I2Cdev library doesn't do this automatically)
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	Serial.println("SPEC_A_2 - Joined I2C.");

	Serial.print("Starte MotorShield.");
	AFMS.begin();  // create with the default frequency 1.6KHz
	Kontrolllauf();
	Serial.print("MotorShield gestartet.");

	Serial.println("Init MPU");
	mpu.Init();
	Serial.println("MPU initialisiert.");

	/*Serial.println("MPU6050-Kalibrierung: ");
	if (mpu.WaitForCalibration(40000) != CALIBRATION_SUCCESS)  // Rückgabewert kann zum Beispiel an Raspberry gesendet werden.
	{
		Serial.println("MPU6050-Kalibrierung gescheitert!");
	}*/
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	for (int i = 0; i < SHARPMEASUREMTS; i++)
	{
		sharplinksvorne.Update();    
		sharprechtsvorne.Update();    // Sensoren nacheinander abfragen, damit diese sich aktualisieren können.
		sharplinkshinten.Update();
		sharprechtshinten.Update();   // Alle Sensoren zu aktualsieren dauert ca. 4400 us.
	}

	serialBuffer.AddMsg(C_SharpLV, sharplinksvorne.GetValue());
	serialBuffer.AddMsg(C_SharpLH, sharplinkshinten.GetValue());
	serialBuffer.AddMsg(C_SharpRV, sharprechtsvorne.GetValue());
	serialBuffer.AddMsg(C_SharpRH, sharprechtshinten.GetValue());
	
	if (ledState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		Wire.setClock(100000); // Change the i2c clock to 100KHz because mlx is to slow for 400 kHz I2C. 
		//TWBR = ((F_CPU / 100000l) - 16) / 2; // 100kHz I2C clock. // maybe this way of changing is even faster.
		serialBuffer.AddMsg(C_MLXLinks, MLXLinks.GetObjTemp());   // Ändern des Wire-Speeds und auslesen eines MLX: ca. 500us (auch mit Klasse)
		serialBuffer.AddMsg(C_MLXVorne, MLXVorne.GetObjTemp());   // Generell müssen die MLX vlt auch nur alle 10 Ticks ausgelesen werden und dann an den RaspberryPi geschickt werden.
		serialBuffer.AddMsg(C_MLXRechts, MLXRechts.GetObjTemp());  // Sollte es zu Timing Problemem kommen, kann auch in einem Tick das MPU ausgelesen werden und im nächsten die Wärme sensoren
		Wire.setClock(400000); // 400kHz I2C clock. Go back to "fullspeed" for MPU and Motorshield.
		//TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz 
	}
	
	mpu.Update();
	if (mpu.NewDataAvaible())
	{
		serialBuffer.AddMsg(C_MPUYaw, mpu.GetYaw(), 8);
		serialBuffer.AddMsg(C_MPUPitch, mpu.GetPitch(), 8);
		serialBuffer.AddMsg(C_MPURoll, mpu.GetRoll(), 8);
	}

	swtLinks = digitalRead2f(switchLinks_Pin);
	swtRechts = digitalRead2f(switchRechts_Pin);  // auch bei extremst kurzem Betätigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	serialBuffer.AddMsg(C_SwitchLinks, swtLinks);
	serialBuffer.AddMsg(C_SwitchRechts, swtRechts);

	serialBuffer.Flush();

	zwei = micros();

	Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife führt zu keinen Fifo-Overflows!
	Serial.println(" us."); 

	//functions.handleSerial();   // Später werden damit die ankommenden seriellen Nachrichten analysiert.
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
						 //Serial1.println(returnvalue);  // Nur für Testzwecke

	for (int i = 1; i < 4; i++)   // Nur die Zahlen analysieren.
	{
		//Serial.print(Value[i]);
		if (Value[i] != ' ')    // Nochmal eine Kontrolle
		{
			returnvalue = returnvalue * 10;
			returnvalue = returnvalue + Value[i] - 48;
		}
		//Serial.println(returnvalue);    //Nur für Testzwecke
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
		//Serial.println("Fehler in der Datenübertragung");
	}

	//Serial.println(returnvalue);        //Nur für Testzwecke
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
				Message[pos] = c;  // Tatsächliche Informationen in einen MessageBuffer kopieren.
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
		/*Serial.print("\tR:");     //Nur für Testzwecke
		Serial.println(r);
		Serial.print("\tL:");     //Nur für Testzwecke
		Serial.println(l); */
		//////


		digitalWrite(13, LOW);
	}
	Serial.flush();

}

