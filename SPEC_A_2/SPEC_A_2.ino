/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** // todo: Was funktioniert gerade?
***
*** Moritz Hauff, 16.02.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
// todo: add necessary includes

///////////////////////////////////////////////////////////////////////////
///Constants
const int LED_Pin = 32;
const int DELAY_TIME = 100;


///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{
	pinMode(LED_Pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	digitalWrite(LED_Pin, HIGH);
	delay(DELAY_TIME);
	digitalWrite(LED_Pin, LOW);
	delay(DELAY_TIME);
}
