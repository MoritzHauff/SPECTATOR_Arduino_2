/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek.
***
*** Test der Sharp-SensorKlasse: http://playground.arduino.cc/Main/SharpIR
***
*** Moritz Hauff, 16.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions

#include "Functions.c"

#include <SharpIR.h>  


///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur für ihren im Setup angegeben Zweck nutzen, sonst kann es zur Beschädigung des ATmegas2560 kommen!

const uint8_t analog_pin = A1;

const uint8_t DELAY_TIME = 100;

///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

//int sensorValue[10];
int sensorValue;

SharpIR sharp(A1, 430);


///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	pinMode2f(led_pin, OUTPUT);

	pinMode(analog_pin, INPUT);

	Serial.begin(115200);
	Serial.println("SPEC_A_2 - Serial Start");
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	/*for (int i = 0; i < 10; i++)
	{
		sensorValue = analogRead(analog_pin);    // 10 Messungen in einer for-Schleife brauchen genau 1120 us.
	}*/
	/*sensorValue[0] = analogRead(analog_pin);    // 10 hardgecodede Messungen benötigen ebenfalls 1120 us.
	sensorValue[1] = analogRead(analog_pin);
	sensorValue[2] = analogRead(analog_pin);
	sensorValue[3] = analogRead(analog_pin);
	sensorValue[4] = analogRead(analog_pin);
	sensorValue[5] = analogRead(analog_pin);
	sensorValue[6] = analogRead(analog_pin);
	sensorValue[7] = analogRead(analog_pin);
	sensorValue[8] = analogRead(analog_pin);
	sensorValue[9] = analogRead(analog_pin);*/
	sensorValue = sharp.distance();   // Mit der Sharp-Klasse und 10(in lib einstellen!) Messungen werden schon 1620 us gebraucht.

	zwei = micros();
	
	Serial.print(" ");
	Serial.print(sensorValue);
	Serial.print(" Zeit: ");
	Serial.print(zwei - eins);
	Serial.println(" us.");

	digitalWrite2f(led_pin, HIGH);   // LOOP-Dauer messbar: konstant leuchtend: <3ms, blinken: ~10 ms
	delay(10);
	digitalWrite2f(led_pin, LOW);
}
