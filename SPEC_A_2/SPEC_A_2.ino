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
#include "S_Sense.h"
#include "SPECTATORClass.h"
#include "Overwatcher.h"

#include "StateMachine.h"

 
///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

///////////////////////////////////////////////////////////////////////////
///Instanzen
StateMachineClass *stateMachine;

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{		
	SA.Init();
	
	stateMachine = new StateMachineClass(&SA);
	stateMachine->Init();	

	OW.Init(stateMachine);
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	
	// todo: StateMachine zeitlich ausmessen.
	stateMachine->DoAction();

	zwei = micros();

	/*Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife führt zu keinen Fifo-Overflows!
	Serial.println(" us.");*/

	eins = micros();
	
	// todo: Zeitmessung der StateMachine!
	OW.Control();

	/*Serial.print("Bisher durchgefuehrte Aktionen: ");
	Serial.println(OW.GetActions());*/

	zwei = micros();

	/*Serial.print("ToF-Zeit: ");    // Die Zeit auf serielle Daten zu überprüfen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");*/        // Das bedeutet die Analyse des seriellen Streams benötigt so sehr wenig Zeit.

	//delay(1000);
}
