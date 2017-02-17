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
 
#include "SharpIR.h"


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

SharpIR sharplinks(analog_pin, SHARPMEASUREMTS);
SharpIR sharprechts(A2, SHARPMEASUREMTS);

int sensorValue;


///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	Serial.begin(115200);
	Serial.println("SPEC_A_2 - Serial Start");

	pinMode2f(led_pin, OUTPUT);

}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	for (int i = 0; i < SHARPMEASUREMTS; i++)
	{
		sharplinks.Update();    
		sharprechts.Update();   // Sensoren nacheinander abfragen, damit diese sich aktualisieren können.
	}

	sensorValue = sharprechts.GetValue();
	sensorValue = sharplinks.GetValue();   // komplett in neue SharpIR-Klasse gepackt, mit 10 Werten: 1200 us.

	zwei = micros();

	Serial.print(" ");
	Serial.print(sensorValue);  
	Serial.print(" Zeit: ");
	Serial.print(zwei - eins);  // Zwei Sharp-Sensoren mit 8 Messungen abzufragen dauert: 1920 us.
	Serial.println(" us.");

	digitalWrite2f(led_pin, HIGH);   // LOOP-Dauer messbar: konstant leuchtend: <3ms, blinken: ~10 ms
	delay(10);
	digitalWrite2f(led_pin, LOW);
}
