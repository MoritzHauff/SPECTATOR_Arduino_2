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
#include <Stepper.h>
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

// change this to the number of steps on your motor
#define STEPS 513

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 4, 5, 6, 7);

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{	
	SA.Init();
	
	stateMachine = new StateMachineClass(&SA);
	stateMachine->Init();
	
	SA.Motoren.TurnLEDOn();
	//SA.Motoren.TurnRescueOn();

	// set the speed of the motor to 30 RPMs
	stepper.setSpeed(60);
}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	
	// todo: StateMachine zeitlich ausmessen.
	//stateMachine->DoAction();

	zwei = micros();

	Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife führt zu keinen Fifo-Overflows!
	Serial.println(" us.");

	eins = micros();
	
	// todo: Zeitmessung der StateMachine!

	zwei = micros();

	/*Serial.print("ToF-Zeit: ");   // Die Zeit auf serielle Daten zu überprüfen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");*/        // Das bedeutet die Analyse des seriellen Streams benötigt so sehr wenig Zeit.

	//delay(10000);

	for (int i = 0; i < 10; i++)
	{
		Serial.println("Forward");
		stepper.step(STEPS);
		delay(2000);
	}
	Serial.println("Backward");
	stepper.step(-STEPS);
	delay(2000);
}
