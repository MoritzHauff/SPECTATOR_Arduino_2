/** StateMachine.cpp
***
*** Diese Klasse überwacht die aktuellen Aufgaben des Roboters.
*** Analysiert außerdem die seriellen Befehle.
***
*** Moritz Hauff, 17.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "StateMachine.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
StateMachineClass::StateMachineClass(SPECTATORClass *Spectator)
{
	spectator = Spectator;

	s_Idle = new S_IdleClass(spectator, "Idle");
	s_TeleOp = new S_TeleOpClass(spectator, "TeleOp");
	s_Drehen = new S_DrehenClass(spectator, "Drehen");
	s_CoffeeBreak = new S_CoffeeBreakClass(spectator, "CoffeeBreak");
	s_Calibrate = new S_CalibrateClass(spectator, "Calibrate");
	s_GeradeAus = new S_GeradeAusClass(spectator, "GeradeAus");
	s_Sense = new S_SenseClass(spectator, "Sense");
	s_SchwarzesFeld = new S_SchwarzesFeldClass(spectator, "SchwarzesFeld");
	
	currentState = 0;

	startedNewAction = false;
}

void StateMachineClass::Init()
{
	changeState(s_Idle);
	s_TeleOp->MotorSpeedL = 0;
	s_TeleOp->MotorSpeedR = 0;
}

StateMachineClass::~StateMachineClass()
{
	// clean-up
	delete currentState;
	delete s_TeleOp;
	delete s_Drehen;
	delete s_CoffeeBreak;
	delete s_Calibrate;
	delete s_GeradeAus;
	delete s_Sense;
	delete s_Idle;
	delete s_SchwarzesFeld;
}

///////////////////////////////////////////////////////////////////////////
///Functions
void StateMachineClass::handleSerial()
{
	char buffer[SERIALBUFF_SIZE + 1]; // +1 allows space for the null terminator
	int pos = 0; // number of characters currently in the buffer

	while (Serial.available())
	{
		char c = Serial.read();

		//Serial.print((byte)c);
		//Serial.print(" ");

		if ((c == '\r') || (c == '\n'))  // Nachrichten müssen daher auch unbedingt mit einem Zeilenende versehen werden!
		{
			// end-of-line received
			/*Serial.print("length=");
			Serial.print(length);    // manchmal ist beim ende der nachricht die länge falsch (=0)
			if (length > 0)
			{
			Serial.println(" >>");
			handleReceivedMessage(buffer);
			}
			length = 0;*/

			handleReceivedMessage(buffer);
			pos = 0;

			//Serial.println(" >>");
		}
		else
		{
			if (pos < SERIALBUFF_SIZE)
			{
				buffer[pos++] = c; // append the received character to the array
				buffer[pos] = 0; // append the null terminator
			}
			else
			{
				// buffer full - discard the received character
			}
		}
	}

	// if there was no newline analyse you could analyse it her anyway
	//handleReceivedMessage(buffer);
}

int StateMachineClass::convertCharToVorzeichen(char c)
{
	if (c == '+')
	{
		return 1;
	}
	else if (c == '-')
	{
		return -1;
	}
	else
	{
		// todo: Fehler ausgeben.
		return 0;
	}
}

//Protokoll für manuelle MotorenSteuerung: C_TELEOPSTART +/- ValueLinks +/- ValueRechts C_TELEOPSTOP
// Drehung: C_TELEOPSTART d n/o/s/w C_TELEOPSTOP
// Felddurchquerung: C_TELEOPSTART g +/- C_TELEOPSTOP
void StateMachineClass::handleReceivedMessage(char *msg)
{
	//Debug
	//Serial.println("Analysiere Daten");

	/*for (int i = 0; i < SERIALBUFF_SIZE; i++)
	{
	Serial.print((byte)msg[i]);
	Serial.print(" ");
	}
	Serial.println(" >>");*/

	if (msg[0] == 'B' && msg[1] == 'R' && msg[2] == 'E' && msg[3] == 'A' && msg[4] == 'K')
	{
		StateClass *s = currentState;  // save the current State into the CoffeeBreaker.
		changeState(s_CoffeeBreak);
		if (s != s_CoffeeBreak)
		{
			s_CoffeeBreak->LastState = s;
		}
	}
	else if (msg[0] == 'C' && msg[1] == 'O' && msg[2] == 'N' && msg[3] == 'T' && msg[4] == 'I' && msg[5] == 'N' && msg[6] == 'U' && msg[7] == 'E' && currentState == s_CoffeeBreak)  // todo: Achtung CoffeeBreak.LastState kann NULL sein.
	{
		resumeState(s_CoffeeBreak->LastState, s_CoffeeBreak->GetTimerShiftAmount());
	}
	else if (msg[0] == C_TELEOPSTART && msg[1] == 'I' && msg[2] == 'D' && msg[3] == 'L' && msg[4] == C_TELEOPSTOP)
	{
		changeState(s_Idle);
	}
	else if (msg[0] == C_TELEOPSTART && msg[1] == 'C' && msg[2] == 'A' && msg[3] == 'L' && msg[4] == C_TELEOPSTOP)
	{
		changeState(s_Calibrate);
	}
	else if (msg[0] == C_TELEOPSTART && msg[1] == 'S' && msg[2] == 'E' && msg[3] == 'N' && msg[4] == C_TELEOPSTOP)
	{
		changeState(s_Sense);
	}
	else if (msg[0] == C_TELEOPSTART && msg[1] == 'S' && msg[2] == 'F' && msg[3] == 'R' && msg[4] == C_TELEOPSTOP)
	{
		changeState(s_SchwarzesFeld);
		s_SchwarzesFeld->ExpectedNumberOfEncoderTicks = s_GeradeAus->GetEncoderL(); // todo maybe use both sides
		spectator->GeradeSchwarzesFeldBefahren = true;
	}
	else if (msg[0] == C_TELEOPSTART && msg[1] == 'g' && msg[3] == C_TELEOPSTOP)
	{
		s_GeradeAus->Direction = convertCharToVorzeichen(msg[2]);
		changeState(s_GeradeAus);
	}
	else if (msg[0] == C_TELEOPSTART)
	{
		if (msg[5] == C_TELEOPSTOP)
		{
			// todo: genauen Modus entscheiden.
			changeState(s_TeleOp);
			
			s_TeleOp->MotorSpeedL = convertCharToVorzeichen(msg[1]) * (byte)msg[2];
			s_TeleOp->MotorSpeedR = convertCharToVorzeichen(msg[3]) * (byte)msg[4];

			/*Serial.print("Neue Motordaten erhalten: l=");
			Serial.print(l);
			Serial.print(" r=");
			Serial.println(r);*/
		}
		if (msg[3] == C_TELEOPSTOP)
		{
			if (msg[1] == 'd')
			{
				changeState(s_Drehen);
				if (msg[2] < 10)
				{
					s_Drehen->ZielRichtung = (int)msg[2];
				}
				else
				{
					s_Drehen->ZielRichtung = spectator->mpuFahrer.CharToRichtung(msg[2]);
				}
				
				spectator->AktRichtung = s_Drehen->ZielRichtung;
				//Serial.print("Neue Richtung gesetzt: ");
				//Serial.println(spectator->AktRichtung);
			}
		}
	}
}

void StateMachineClass::SendDirectCommand(char *msg)
{
	handleReceivedMessage(msg);
}

///////////////////////////////////////////////////////////////////////////
///State-Functions
void StateMachineClass::changeState(StateClass *NextState)
{
	Serial.print("Started Next State: "); // to inform the Rapi 
	Serial.println(NextState->GetName());  
	startedNewAction = true;
	
	if (currentState != NextState)
	{
		currentState = NextState;
	}

	// Zurücksetzen aller Timer und Umgebungsvariablen beim neustarten eines Modus.
	if (currentState == s_Idle)
	{
		s_Idle->Init();  // mit currentState werden die Variablen nicht korrekt initialisiert.
	}
	else if (currentState == s_TeleOp)
	{
		s_TeleOp->Init();
	}
	else if (currentState == s_Drehen)
	{
		s_Drehen->Init();
	}
	else if (currentState == s_CoffeeBreak)
	{
		s_CoffeeBreak->Init();
	}
	else if (currentState == s_Calibrate)
	{
		s_Calibrate->Init();
	}
	else if (currentState == s_GeradeAus)
	{
		s_GeradeAus->Init();
	}
	else if (currentState == s_Sense)
	{
		s_Sense->Init();
	}
	else if (currentState == s_SchwarzesFeld)
	{
		s_SchwarzesFeld->Init();
	}
}

void StateMachineClass::resumeState(StateClass *NextState, unsigned long TimerShiftAmount)
{
	if (currentState != NextState)
	{
		currentState = NextState;

		if (currentState == s_Idle)
		{
			s_Idle->ShiftTimers(TimerShiftAmount);
		}
		else if (currentState == s_TeleOp)
		{
			s_TeleOp->ShiftTimers(TimerShiftAmount);
		}
		else if (currentState == s_Drehen)
		{
			s_Drehen->ShiftTimers(TimerShiftAmount);
		}
		else if (currentState == s_Calibrate)
		{
			s_Calibrate->ShiftTimers(TimerShiftAmount);
		}
		else if (currentState == s_GeradeAus)
		{
			s_GeradeAus->ShiftTimers(TimerShiftAmount);
		}
		else if (currentState == s_Sense)
		{
			s_Sense->ShiftTimers(TimerShiftAmount);
		}
		else if (currentState == s_SchwarzesFeld)
		{
			s_SchwarzesFeld->ShiftTimers(TimerShiftAmount);
		}

		spectator->Motoren.TurnLEDOn(); // Nach der KaffeePause könnte die LED ausgeschaltet sein.

		Serial.println(String("Modus wieder aufgenommen: " + currentState->GetName()));  // Debugging
	}
}

void StateMachineClass::checkStates()
{
	if (currentState->GetStatus() == Finished)
	{
		Serial.print("Finished Last State: ");
		Serial.println(currentState->GetName());
	}

	if (currentState->GetStatus() == Error && currentState == s_Drehen)
	{
		OverwatcherMsg("DrehFehler");
	}

	if (currentState->GetStatus() == Finished &&( currentState == s_Drehen || currentState == s_Idle))
	{
		changeState(s_Sense);  // Nach jedem Drehen oder längere Pause automatisch Feld erfassen
	}
	else if (currentState->GetStatus() == Finished && currentState == s_GeradeAus)
	{
		changeState(s_Drehen);  // Nach jedem geradeaus fahren automatisch drehen
		//s_Drehen->ZielRichtung = spectator->mpuFahrer.CalculateRichtung(spectator->mpu.GetYaw());
		s_Drehen->ZielRichtung = spectator->AktRichtung; // beim S_Drehen.Init() wird die richtung zurückgesetzt.  // solange aber keine neuen Befhel kommen sollte die im spectator gepseicherte Richtung die richtige sein.
	}
	else if (currentState->GetStatus() == Finished || currentState->GetStatus() == Aborted || currentState->GetStatus() == Error)
	{
		changeState(s_Idle);   // todo add some recover strategies
	}
}

void StateMachineClass::DoAction()
{
	handleSerial();

	checkStates();

	// Do the real Action.
	if (currentState == s_Idle)
	{
		s_Idle->Sense();   
		s_Idle->Think();
		s_Idle->Act();
	}
	else if (currentState == s_TeleOp) 
	{
		/*currentState->Sense();
		currentState->Think();
		currentState->Act();*/
		s_TeleOp->Sense();   // wird über die gecastete Instanz 'currentState' zugegriffen werden Variablen die innerhalb des States definiert und verwendet werden (wie z.B. Toggle) nicht korrekt angesprochen.
		s_TeleOp->Think();
		s_TeleOp->Act();
	}
	else if (currentState == s_Drehen)
	{
		s_Drehen->Sense();
		s_Drehen->Think();
		s_Drehen->Act();
	}
	else if (currentState == s_CoffeeBreak)
	{
		s_CoffeeBreak->Sense();
		s_CoffeeBreak->Think();
		s_CoffeeBreak->Act();
	}
	else if (currentState == s_Calibrate)
	{
		/*if (s_Calibrate->GetStatus() == Finished)
		{
			changeState(s_TeleOp); // todo: Should be s_Idle.
		}
		else*/
		{
			s_Calibrate->Sense();
			s_Calibrate->Think();
			s_Calibrate->Act();
		}
	}
	else if (currentState == s_GeradeAus)
	{
		s_GeradeAus->Sense();
		s_GeradeAus->Think();
		s_GeradeAus->Act();
	}
	else if (currentState == s_Sense)
	{
		s_Sense->Sense();
		s_Sense->Think();
		s_Sense->Act();
	}
	else if (currentState == s_SchwarzesFeld)
	{
		s_SchwarzesFeld->Sense();
		s_SchwarzesFeld->Think();
		s_SchwarzesFeld->Act();
	}
}

bool StateMachineClass::StartedNewAction()
{
	if (startedNewAction == true)
	{
		startedNewAction = false;
		return true;
	}

	return false;
}
