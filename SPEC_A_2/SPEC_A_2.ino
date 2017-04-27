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
#include "MovementArray.h"
#include "MovementArrays.h"
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
///SRAM-Funktion
/*Diese Funktion gibt die Menge des aktuell unbenutzten SRAMs zur�ck. 
F�r weitere Informationen siehe: Adafruit.*/
int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

/// <summary>
/// Diese Funktion gibt die Menge des aktuell unbenutzen SRAMs �ber
/// die serielle Schnittstelle aus. Diese muss davor gegebenfalls gestartet werden.
/// </summary>
void printFreeRam()
{
	Serial.print("Unused RAM: ");
	Serial.println(freeRam(), DEC);
}

///////////////////////////////////////////////////////////////////////////
///Setup
void setup()
{		
	SA.Init();
	
	stateMachine = new StateMachineClass(&SA);
	stateMachine->Init();	

	printFreeRam();

	OW.Init(stateMachine);

	printFreeRam();

	MovementArray.GoToStart();
	MotorDaten *md;
	do {
		md = MovementArray.GetNextMovement(bumperLinks, bumperLinksRows);
		if (md != NULL)
		{
			Serial.print(md->MotorSpeedL);
			Serial.print(" ");
			Serial.println(md->MotorSpeedR);
		}
	} while (md != NULL);
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
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife f�hrt zu keinen Fifo-Overflows!
	Serial.println(" us.");*/

	eins = micros();
	
	// todo: Zeitmessung des Overwatchers
	OW.Control();

	/*Serial.print("Bisher durchgefuehrte Aktionen: ");
	Serial.println(OW.GetActions());*/

	zwei = micros();

	/*Serial.print("ToF-Zeit: ");    // Die Zeit auf serielle Daten zu �berpr�fen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");*/        // Das bedeutet die Analyse des seriellen Streams ben�tigt so sehr wenig Zeit.

	//delay(1000);
}
