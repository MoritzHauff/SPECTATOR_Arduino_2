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
#include "HCSr04.h"
#include "SPECTATORClass.h"
#include "Constants.h"
#include <arduino2.h>

#include "StateMachine.h"
 
///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

///////////////////////////////////////////////////////////////////////////
///Instanzen
StateMachineClass *stateMachine;

HCSr04_InterruptClass usInterrupt(18, DP24);
HCSr04_InterruptClass usInterrupt2(19, DP22);

static void ISRUSH()   // for the interrupt callback you need a real function
				// for more information see: http://stackoverflow.com/questions/400257/how-can-i-pass-a-class-member-function-as-a-callback
{
	//Serial.println("Interrupt");
	/*if (usInterrupt) // check if it was initialized
	{
		usInterrupt->HandleInterrupt();
	}*/
	usInterrupt.HandleInterrupt();
}

static void ISRUSV()
{
	//Serial.println("Interrupt");
	/*if (usInterrupt2) // check if it was initialized
	{
		usInterrupt2->HandleInterrupt();
	}*/
}

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	SA.Init();
	
	stateMachine = new StateMachineClass(&SA);
	stateMachine->Init();

	usInterrupt.Init();
	usInterrupt2.Init();

	attachInterrupt(digitalPinToInterrupt(18), ISRUSH, CHANGE);
	attachInterrupt(digitalPinToInterrupt(19), ISRUSV, CHANGE);
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	
	// todo: StateMachine zeitlich ausmessen.
	//stateMachine->DoAction();

	zwei = micros();

	/*Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife führt zu keinen Fifo-Overflows!
	Serial.println(" us.");*/

	eins = micros();
	
	// todo: Zeitmessung der StateMachine!
	/*SA.ultraschallRechts.Update();  // abhängig von der entfernung 10000 us - >25000 us
	Serial.print("Rechts: ");
	Serial.println(SA.ultraschallRechts.GetDistance());*/
	if (usInterrupt.IsFinished())
	{
		//usInterrupt.Update();
		Serial.print("Hinten: ");
		Serial.println(usInterrupt.GetDistance());
		usInterrupt.StartMeasurement();
	}
	/*if (usInterrupt2->IsFinished())
	{
		Serial.print("Vorne: ");
		Serial.println(usInterrupt2->GetDistance());
		usInterrupt2->StartMeasurement();
	}*/

	zwei = micros();

	/*Serial.print("Ultraschall-Zeit: ");   // Die Zeit auf serielle Daten zu überprüfen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");*/          // Das bedeutet die Analyse des seriellen Streams benötigt so sehr wenig Zeit.

	//delay(10000);
}
