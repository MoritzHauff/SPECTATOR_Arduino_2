/** SPECTATORClass.cpp
***
*** Hier laufen alle andern ben�tigten Instanzen und die dazugeh�rigen 
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

///////////////////////////////////////////////////////////////////////////
///Konstruktoren 
void SPECTATORClass::Init()
{
	// Digital Pins
	HeartbeatLED.Init();
	switchLinks.Init();
	switchRechts.Init();

	// ultrasonic-distance-sensors
	ultraschallLinks.Init();
	ultraschallVorne.Init();
	ultraschallRechts.Init();
	ultraschallHinten.Init();

	// attach the ultrasonic-sensor interrupts
	attachInterrupt(digitalPinToInterrupt(ultraschallVorne.GetEchoPin()), ISRUSV, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ultraschallHinten.GetEchoPin()), ISRUSH, CHANGE);

	// Serial communication
	Serial.begin(250000);  // Je h�her die Baudrate und je mehr Daten im Serial.print stehen desto mehr Zeit wird gespart.
	Serial.println("SPEC_A_2 - Serial Start");

	// init Laser
	laserVorne.Init();  // start this version of wire before the final one later (see below).

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

	//Motoren.TurnLEDOn();

	//MPUCalibration();  // todo: sollte durch RaPi ausgel�st werden!
}

void SPECTATORClass::MPUCalibration()
{
	Serial.println("MPU6050-Kalibrierung...");
	if (mpu.WaitForCalibration(40000) != CALIBRATION_SUCCESS)  // R�ckgabewert kann zum Beispiel an Raspberry gesendet werden.
	{
		Serial.println("MPU6050-Kalibrierung gescheitert!");
	}

	mpu.Update();
	Serial.print("aktueller Yaw-Wert: ");
	Serial.println(mpu.GetYaw());
	
	mpuFahrer.SetNorden(mpu.GetYaw());   // hardcode the mpuFahrerCalibration // todo: should normally done be the RaPi.
	
	Serial.println("MPU6050 kalibriert.");
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void SPECTATORClass::UpdateSwitches()  // 128 us
{
	switchLinks.Update();
	switchRechts.Update();   // auch bei extremst kurzem Bet�tigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	serialBuffer.AddMsg(C_SwitchLinks, switchLinks.GetLastState());
	serialBuffer.AddMsg(C_SwitchRechts, switchRechts.GetLastState());
}

void SPECTATORClass::UpdateSharp()
{
	for (int i = 0; i < SHARPMEASUREMTS; i++)
	{
		sharplinksvorne.Update();
		sharprechtsvorne.Update();    // Sensoren nacheinander abfragen, damit diese sich aktualisieren k�nnen.
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
	SA.serialBuffer.AddMsg(C_MLXLinks, SA.MLXLinks.GetObjTemp());    // �ndern des Wire-Speeds und auslesen eines MLX: ca. 500us (auch mit Klasse)
	SA.serialBuffer.AddMsg(C_MLXVorne, SA.MLXVorne.GetObjTemp());    // Generell m�ssen die MLX vlt auch nur alle 10 Ticks ausgelesen werden und dann an den RaspberryPi geschickt werden.
	SA.serialBuffer.AddMsg(C_MLXRechts, SA.MLXRechts.GetObjTemp());  // Sollte es zu Timing Problemem kommen, kann auch in einem Tick das MPU ausgelesen werden und im n�chsten die W�rme sensoren
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
	laserVorne.Update();
	if (laserVorne.NewDataAvaiable())
	{
		serialBuffer.AddMsg(C_LaserV, laserVorne.GetDistance());
	}
}
