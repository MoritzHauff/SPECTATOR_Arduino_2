/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek von: 
*** https://www.codeproject.com/Articles/732646/Fast-digital-I-O-for-Arduino
***
*** Moritz Hauff, 16.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#define  GPIO2_PREFER_SPEED    1   // results in even faster execution
#include <arduino2.h>   // include the fast I/O 2 functions


///////////////////////////////////////////////////////////////////////////
///Constants
// The I/O 2 functions use special data type for pin
// Pin codes, such as DP13 are defined in pins2_arduino.h
const GPIO_pin_t led_pin = DP32;  // Achtung: Pins nur für ihren im Setup angegeben Zweck nutzen, sonst kann es zur Beschädigung des ATmegas2560 kommen!

const int DELAY_TIME = 100;


///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{
	pinMode2f(led_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	digitalWrite2f(led_pin, HIGH);
	delay(DELAY_TIME);
	digitalWrite2f(led_pin, LOW);
	delay(DELAY_TIME);
}
