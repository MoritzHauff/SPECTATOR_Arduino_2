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
const GPIO_pin_t switchLinks_Pin = DP23;
const GPIO_pin_t switchRechts_Pin = DP25;

const uint8_t analog_Pin = A1;

const uint8_t DELAY_TIME = 100;

const uint8_t SHARPMEASUREMTS = 8;   // acht sharp-Messungen an einem Sensor dauern genau 940 us. Diese reium an allen würden damit 4 ms dauern.

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

SerialBuffer serialBuffer = SerialBuffer();

float sensorValue;
bool ledState = false;

bool swtLinks = false;
bool swtRechts = false;

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


	ledState = !ledState;
	digitalWrite2f(led_pin, ledState);   // Make the heartbeat.
}

