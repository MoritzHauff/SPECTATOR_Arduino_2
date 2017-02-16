/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek.
***
*** Test des schnelleren analogReads() von: 
*** https://bennthomsen.wordpress.com/arduino/peripherals/analogue-input/
***
*** Moritz Hauff, 16.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions

#include "Functions.c"


///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur für ihren im Setup angegeben Zweck nutzen, sonst kann es zur Beschädigung des ATmegas2560 kommen!

const uint8_t analog_pin = A0;

const uint8_t DELAY_TIME = 100;

///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

uint8_t sensorValue = 0;


///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{
	init_adc();
	
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
	sensorValue = analogRead(analog_pin);
	zwei = micros();

	Serial.print(sensorValue);
	Serial.print(" Zeit: ");
	Serial.print(zwei - eins);
	Serial.print(" us.");

	eins = micros();
	sensorValue = get_adc(analog_pin);
	zwei = micros();
	
	Serial.print(" ");
	Serial.print(sensorValue);
	Serial.print(" Zeit: ");
	Serial.print(zwei - eins);
	Serial.println(" us.");

	digitalWrite2f(led_pin, HIGH);
	delay(10);
	digitalWrite2f(led_pin, LOW);
}
