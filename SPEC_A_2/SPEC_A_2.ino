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
#include "SPECTATORClass.h"
#include "Constants.h"
 
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
///Setup
void setup()
{	
	SA.Init();


}

///////////////////////////////////////////////////////////////////////////
///Loop
void loop()
{
	eins = micros();
	
	SA.UpdateSharp();
	
	if (ledState)   // nur jeden zweiten loopDurchgang sollen die MLX ausgelesen werden.
	{
		SA.UpdateMLX();
	}
	
	SA.UpdateMPU();

	swtLinks = digitalRead2f(switchLinks_Pin);
	swtRechts = digitalRead2f(switchRechts_Pin);  // auch bei extremst kurzem Bet�tigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	SA.serialBuffer.AddMsg(C_SwitchLinks, swtLinks);
	SA.serialBuffer.AddMsg(C_SwitchRechts, swtRechts);

	SA.serialBuffer.Flush();
	//SA.serialBuffer.Clear();

	zwei = micros();

	/*Serial.print("loop-Zeit: ");  // 11500 us bei neuen MPU Daten, sonst 6030 us.
	Serial.print(zwei - eins);    // Die jetztige loop-Schleife f�hrt zu keinen Fifo-Overflows!
	Serial.println(" us."); */

	eins = micros();
	functions.handleSerial();   // Sp�ter werden damit die ankommenden seriellen Nachrichten analysiert.
	SA.Motoren.SetMotoren(functions.l, functions.r);
	zwei = micros();

	/*Serial.print("Serial-Zeit: ");   // Die Zeit auf serielle Daten zu �berpr�fen und die Motoren anzusteuern: 
	Serial.print(zwei - eins);       // ohne neue Daten: 1276 us. Beim Eingang neuer Motordaten: 1316 us
	Serial.println(" us.");      */    // Das bedeutet die Analyse des seriellen Streams ben�tigt so sehr wenig Zeit.


	ledState = !ledState;
	digitalWrite2f(led_pin, ledState);   // Make the heartbeat.
}



int c_array_to_int(char Value[4])
{
	int returnvalue;

	//Serial.print("c_Array: ");
	//Serial.println(Value);

	returnvalue = 0;     // Beispielsumrechnung: Char zu int konvertieren, geschieht durch -48
						 //Serial1.println(returnvalue);  // Nur f�r Testzwecke

	for (int i = 1; i < 4; i++)   // Nur die Zahlen analysieren.
	{
		//Serial.print(Value[i]);
		if (Value[i] != ' ')    // Nochmal eine Kontrolle
		{
			returnvalue = returnvalue * 10;
			returnvalue = returnvalue + Value[i] - 48;
		}
		//Serial.println(returnvalue);    //Nur f�r Testzwecke
	}

	if (Value[0] == '-')
	{
		returnvalue = returnvalue * -1;
	}
	else if (Value[0] == '+')
	{
		// do nothing
	}
	else
	{
		//Serial.println("Fehler in der Daten�bertragung");
	}

	//Serial.println(returnvalue);        //Nur f�r Testzwecke
	return returnvalue;
}
