/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek. Und angeschlossener SHARP-Distanzsensoren,
*** sowie des MPU6050s.
***
*** ZeitTests der MLX-Temp-Sensoren.
***
*** Moritz Hauff, 18.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions

#include "SharpIR.h"
#include "MPU.h"
#include "MLX90614Class.h"

#include "SerialBuffer.h"

#include "Functions.c"
 

///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur für ihren im Setup angegeben Zweck nutzen, sonst kann es zur Beschädigung des ATmegas2560 kommen!

const uint8_t analog_pin = A1;

const uint8_t DELAY_TIME = 100;

const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen würden damit 4 ms dauern.

///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

SharpIR sharplinksvorne(analog_pin, SHARPMEASUREMTS);
SharpIR sharprechtsvorne(A2, SHARPMEASUREMTS);
SharpIR sharplinkshinten(A3, SHARPMEASUREMTS);
SharpIR sharprechtshinten(A4, SHARPMEASUREMTS);

MPU mpu = MPU();

MLX90614Class MLXLinks = MLX90614Class(0x2C);
MLX90614Class MLXVorne = MLX90614Class(0x2A);
MLX90614Class MLXRechts = MLX90614Class(0x2B);

SerialBuffer serialBuffer = SerialBuffer();

float sensorValue;
bool ledState = false;



///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	pinMode2f(led_pin, OUTPUT);
	
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

	mpu.Init();

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
		sharprechtsvorne.Update();   // Sensoren nacheinander abfragen, damit diese sich aktualisieren können.
		sharplinkshinten.Update();
		sharprechtshinten.Update();   // Alle Sensoren zu aktualsieren dauert ca. 4400 us.
	}

	int a;
	a = sharplinksvorne.GetValue();
	a = sharprechtsvorne.GetValue();
	a = sharplinkshinten.GetValue();
	a = sharprechtshinten.GetValue();
	//todo: Tue etwas mit den Daten.
	
	Wire.setClock(100000); // Change the i2c clock to 100KHz because mlx is to slow for 400 kHz I2C. 
	//TWBR = ((F_CPU / 100000l) - 16) / 2; // 100kHz I2C clock. // maybe this way of changing is even faster.
	sensorValue = MLXLinks.GetObjTemp();   // Ändern des Wire-Speeds und auslesen eines MLX: ca. 500us (auch mit Klasse)
	sensorValue = MLXVorne.GetObjTemp();   // Generell müssen die MLX vlt auch nur alle 10 Ticks ausgelesen werden und dann an den RaspberryPi geschickt werden.
	sensorValue = MLXRechts.GetObjTemp();  // Sollte es zu Timing Problemem kommen, kann auch in einem Tick das MPU ausgelesen werden und im nächsten die Wärme sensoren

	Wire.setClock(400000); // 400kHz I2C clock. Go back to "fullspeed" for MPU and Motorshield.
	//TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz 
	mpu.Update();


	zwei = micros();

	/*Serial.print(" ");
	Serial.print(sensorValue);  
	Serial.print(" Zeit: ");  // 10500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);
	Serial.println(" us.");*/

	eins = micros();
	serialBuffer.AddMSG(65, 34);
	serialBuffer.Flush();
	zwei = micros();
	drei = zwei - eins;
	eins = micros();
	Serial.println("aA34c");
	zwei = micros();
	Serial.print("mit Klasse: ");  // 139 oder mit dsprintf soagr 140 us
	Serial.print(drei);
	Serial.print(" ohne Klasse: ");   // 80 us. 
	Serial.println(zwei - eins);   // vielleicht sollten die daten gar nicht zusammengeapckt werden sondern einfach an Serial geschickt werden und fertig.


	ledState = !ledState;
	digitalWrite2f(led_pin, ledState);   // Make the heartbeat.
}

