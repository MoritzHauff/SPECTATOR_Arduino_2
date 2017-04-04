/** SPEC_A_2.ino
***
*** Dieses Programm l�uft auf dem SPECTATOR-Arduino und �berwacht dessen Sensoren.
***
*** Test der schnelleren IO-Bibliothek. Und angeschlossener SHARP-Distanzsensoren,
*** sowie des MPU6050s. ZeitTests der MLX-Temp-Sensoren und der Sto�sensoren Vorne.
***
*** Serielle �bertragung aller Sensorwerte und Steuerung der Motoren.
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "HCSr04.h"
#include "SPECTATORClass.h"
#include "Constants.h"

#include "StateMachine.h"
 
///////////////////////////////////////////////////////////////////////////
///Variablen

unsigned long eins = 0;
unsigned long zwei = 0;
unsigned long drei = 0;

///////////////////////////////////////////////////////////////////////////
///Instanzen
StateMachineClass *stateMachine;

//HCSr04_InterruptClass usInterrupt(22, 19);
HCSr04_InterruptClass usInterrupt(24, 18);

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	SA.Init();
	
	stateMachine = new StateMachineClass(&SA);
	stateMachine->Init();

	usInterrupt.begin();
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
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife f�hrt zu keinen Fifo-Overflows!
	Serial.println(" us.");*/

	eins = micros();
	
	// todo: Zeitmessung der StateMachine!
	/*SA.ultraschallRechts.Update();  // abh�ngig von der entfernung 10000 us - >25000 us
	Serial.print("Rechts: ");
	Serial.println(SA.ultraschallRechts.GetDistance());*/
	if (usInterrupt.isFinished())
	{
		Serial.print("Hinten: ");
		Serial.println(usInterrupt.getDistance());
		usInterrupt.start();
	}
	/*if (usInterrupt2.isFinished())
	{
		Serial.print("Vorne: ");
		Serial.println(usInterrupt2.getDistance());
		usInterrupt2.start();
	}*/

	zwei = micros();

	/*Serial.print("Ultraschall-Zeit: ");   // Die Zeit auf serielle Daten zu �berpr�fen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");*/          // Das bedeutet die Analyse des seriellen Streams ben�tigt so sehr wenig Zeit.

	//delay(10000);
}
