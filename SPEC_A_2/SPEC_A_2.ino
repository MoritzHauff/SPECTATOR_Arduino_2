/** SPEC_A_2.ino
***
*** Dieses Programm läuft auf dem SPECTATOR-Arduino und überwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek. Und angeschlossener SHARP-Distanzsensoren,
*** sowie des MPU6050s. ZeitTests der MLX-Temp-Sensoren und der Stoßsensoren Vorne.
***
*** Serielle Übertragung aller Sensorwerte und Steuerung der Motoren.
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECTATORClass.h"
#include "Constants.h"

#include "StateMachine.h"

#include "DigitalIO.h"
 
///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

float sensorValue;
bool ledState = false;

bool swtLinks = false;
bool swtRechts = false;

///////////////////////////////////////////////////////////////////////////
///Instanzen
StateMachineClass stateMachine(&SA);

DigitalIOClass dioled = DigitalIOClass(DP32, OUTPUT);

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	SA.Init();

	dioled.Init();
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	//eins = micros();
	
	// todo: StateMachine zeitlich ausmessen.

	swtLinks = SA.switchLinks.Read();
	swtRechts = SA.switchRechts.Read();  // auch bei extremst kurzem Betätigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	//zwei = micros();  // 8 us.

	SA.serialBuffer.AddMsg(C_SwitchLinks, swtLinks);
	SA.serialBuffer.AddMsg(C_SwitchRechts, swtRechts);

	//stateMachine.DoAction();

	Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife führt zu keinen Fifo-Overflows!
	Serial.println(" us."); 

	eins = micros();
	
	// todo: Zeitmessung der StateMachine!

	zwei = micros();

	//Serial.print("Serial-Zeit: ");   // Die Zeit auf serielle Daten zu überprüfen und die Motoren anzusteuern: 
	//Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	//Serial.println(" us.");          // Das bedeutet die Analyse des seriellen Streams benötigt so sehr wenig Zeit.

	/*eins = micros();
	ledState = !ledState;
	digitalWrite2f(led_pin, ledState);   // Make the heartbeat.
	zwei = micros();*/  // 4 us

	/*eins = micros();
	ledState = !ledState;
	SA.HeartbeatLED.Write(ledState);
	zwei = micros();*/  // 8 us

	eins = micros();
	SA.HeartbeatLED.Toggle();
	zwei = micros(); // 8 us

	//delay(10000);
}
