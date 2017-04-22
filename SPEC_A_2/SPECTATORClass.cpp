/** SPECTATORClass.cpp
***
*** Hier laufen alle andern benötigten Instanzen und die dazugehörigen 
*** Funktionen zusammen.
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECTATORClass.h"

///////////////////////////////////////////////////////////////////////////
///Instanz
SPECTATORClass SA;  // Die HauptInstanz des SPECTATOR-Arduinos.

///////////////////////////////////////////////////////////////////////////
///ISR (statische Funktionen)
static void ISRUSV()   // for the interrupt callback you need a real function not a method.
					   // for more information see: http://stackoverflow.com/questions/400257/how-can-i-pass-a-class-member-function-as-a-callback
{
	SA.ultraschallVorne.HandleInterrupt();
}

static void ISRUSH()
{
	SA.ultraschallHinten.HandleInterrupt();
}

static void ISREL()  // call-back method for the left-wheel-encoder
{
	//Serial.print("EncoderInterrupt. ");   // debug
	SA.Motoren.encoderLinks.HandleInterrupt();
}

static void ISRER()  // call-back method for the right-wheel-encoder
{
	SA.Motoren.encoderRechts.HandleInterrupt();
}


///////////////////////////////////////////////////////////////////////////
///Konstruktoren 
void SPECTATORClass::Init()
{
	// Globale Variablen
	AktRichtung = R_NORDEN;
	GeradeSchwarzesFeldBefahren = false;
	GeradeRampeBefahren = false;

	// Digital Pins
	HeartbeatLED.Init();
	switchLinks.Init();
	switchRechts.Init();

	ldr.Init();

	// ultrasonic-distance-sensors
	ultraschallLinks.Init();
	ultraschallVorne.Init();
	ultraschallRechts.Init();
	ultraschallHinten.Init();

	// attach the ultrasonic-sensor interrupts
	attachInterrupt(digitalPinToInterrupt(ultraschallVorne.GetEchoPin()), ISRUSV, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ultraschallHinten.GetEchoPin()), ISRUSH, CHANGE);

	// Serial communication
	Serial.begin(230400);  // Je höher die Baudrate und je mehr Daten im Serial.print stehen desto mehr Zeit wird gespart. // 250000 kann vom RaspberryPi nicht gelesen werden.
	Serial.println("SPEC_A_2 - Serial Start");

	// init Laser
	laserVorne.Init();  // start this version of 'wire' before the final one later (see below).

	// Join I2C bus (I2Cdev library doesn't do this automatically)
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	Serial.println("SPEC_A_2 - Joined I2C.");

	Serial.println("Init MPU...");
	mpu.Init();
	Serial.println("MPU initialisiert.");

	Motoren.Init();
	Motoren.Kontrolllauf();

	// attach the interrupts for the encoders. Must be called here because SA is the only global instance which can be refenrenced in the static functions.
	attachInterrupt(digitalPinToInterrupt(3), ISREL, CHANGE);
	attachInterrupt(digitalPinToInterrupt(2), ISRER, CHANGE);

	//Motoren.TurnLEDOn();
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void SPECTATORClass::UpdateSwitches()  // 128 us
{
	switchLinks.Update();
	switchRechts.Update();   // auch bei extremst kurzem Betätigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	serialBuffer.AddMsg(C_SwitchLinks, switchLinks.GetLastState());
	serialBuffer.AddMsg(C_SwitchRechts, switchRechts.GetLastState());
}

void SPECTATORClass::UpdateLDR()
{
	ldr.Update();

	serialBuffer.AddMsg(C_LDR, ldr.GetValue());
}

void SPECTATORClass::UpdateSharp()
{
	for (int i = 0; i < SHARPMEASUREMTS; i++)
	{
		sharplinksvorne.Update();
		sharprechtsvorne.Update();    // Sensoren nacheinander abfragen, damit diese sich aktualisieren können.
		sharplinkshinten.Update();
		sharprechtshinten.Update();   // Alle Sensoren zu aktualsieren dauert ca. 4400 us.
	}

	serialBuffer.AddMsg(C_SharpLV, sharplinksvorne.GetValue());
	serialBuffer.AddMsg(C_SharpLH, sharplinkshinten.GetValue());
	serialBuffer.AddMsg(C_SharpRV, sharprechtsvorne.GetValue());
	serialBuffer.AddMsg(C_SharpRH, sharprechtshinten.GetValue());
}

void SPECTATORClass::UpdateMLX()
{
	Wire.setClock(100000); // Change the i2c clock to 100KHz because mlx is to slow for 400 kHz I2C. 
	//TWBR = ((F_CPU / 100000l) - 16) / 2; // 100kHz I2C clock. // maybe this way of changing is even faster.
	SA.serialBuffer.AddMsg(C_MLXLinks, SA.MLXLinks.GetObjTemp());    // Ändern des Wire-Speeds und auslesen eines MLX: ca. 500us (auch mit Klasse)
	SA.serialBuffer.AddMsg(C_MLXVorne, SA.MLXVorne.GetObjTemp());    // Generell müssen die MLX vlt auch nur alle 10 Ticks ausgelesen werden und dann an den RaspberryPi geschickt werden.
	SA.serialBuffer.AddMsg(C_MLXRechts, SA.MLXRechts.GetObjTemp());  // Sollte es zu Timing Problemem kommen, kann auch in einem Tick das MPU ausgelesen werden und im nächsten die Wärme sensoren
	Wire.setClock(400000); // 400kHz I2C clock. Go back to "fullspeed" for MPU and Motorshield.
	//TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz 
}

void SPECTATORClass::UpdateMPU()
{
	mpu.Update();
	if (mpu.NewDataAvaible())
	{
		serialBuffer.AddMsg(C_MPUYaw, mpu.GetYaw(), 8);
		serialBuffer.AddMsg(C_MPUPitch, mpu.GetPitch(), 8);
		serialBuffer.AddMsg(C_MPURoll, mpu.GetRoll(), 8);
	}
}

void SPECTATORClass::UpdateHCSr04Seitlich()
{
	ultraschallLinks.Update();
	ultraschallRechts.Update();

	serialBuffer.AddMsg(C_UltraschallL, ultraschallLinks.GetDistance());
	serialBuffer.AddMsg(C_UltraschallR, ultraschallRechts.GetDistance());
}

void SPECTATORClass::UpdateHCSr04VorneHinten()
{
	ultraschallVorne.Update();
	if (ultraschallVorne.NewDataAvaible())
	{
		serialBuffer.AddMsg(C_UltraschallV, ultraschallVorne.GetDistance());
	}

	ultraschallHinten.Update();
	if (ultraschallHinten.NewDataAvaible())
	{
		serialBuffer.AddMsg(C_UltraschallH, ultraschallHinten.GetDistance());
	}
}

void SPECTATORClass::UpdateLaser()
{
	laserVorne.Reset();
	laserVorne.Update();
	if (laserVorne.NewDataAvaiable())
	{
		serialBuffer.AddMsg(C_LaserV, laserVorne.GetDistance());
	}
}

void SPECTATORClass::UpdateEncoder()
{
	Motoren.UpdateWheelEncoderInfo();

	EncoderInfo encoderInfo = Motoren.GetEncoderInfoL();

	serialBuffer.AddMsg(C_EncoderLC, encoderInfo.CountsSinceLastTick);
	serialBuffer.AddMsg(C_EncoderLT, encoderInfo.MillisSinceLastTick);

	encoderInfo = Motoren.GetEncoderInfoR();

	serialBuffer.AddMsg(C_EncoderRC, encoderInfo.CountsSinceLastTick);
	serialBuffer.AddMsg(C_EncoderRT, encoderInfo.MillisSinceLastTick);
}

bool SPECTATORClass::GetWand(byte Direction)
{
	if (Direction == R_VORNE)
	{
		if (ultraschallVorne.GetDistance() < 8)
		{
			return true;
		}
	}
	if (Direction == R_RECHTS)
	{
		if (ultraschallRechts.GetDistance() < 8)
		{
			return true;
		}
	}
	if (Direction == R_HINTEN)
	{
		if (ultraschallHinten.GetDistance() < 8)
		{
			return true;
		}
	}
	if (Direction == R_LINKS)
	{
		if (ultraschallLinks.GetDistance() < 8)
		{
			return true;
		}
	}

	return false;
}