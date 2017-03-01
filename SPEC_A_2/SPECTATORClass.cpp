/** SPECTATORClass.cpp
***
*** Hier laufen alle andern ben�tigten Instanzen und die dazugeh�rigen 
*** Funktionen zusammen
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECTATORClass.h"

///////////////////////////////////////////////////////////////////////////
///Instanz
SPECTATORClass SA;  // Die HauptInstanz des SPECTATOR-Arduinos.

void SPECTATORClass::Init()
{
	pinMode2f(led_pin, OUTPUT);
	pinMode2f(switchLinks_Pin, INPUT);
	pinMode2f(switchRechts_Pin, INPUT);

	Serial.begin(250000);  // Je h�her die Baudrate und je mehr Daten im Serial.print stehen desto mehr Zeit wird gespart.
	Serial.println("SPEC_A_2 - Serial Start");

	// join I2C bus (I2Cdev library doesn't do this automatically)
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	Serial.println("SPEC_A_2 - Joined I2C.");

	Serial.println("Init MPU");
	mpu.Init();
	Serial.println("MPU initialisiert.");

	/*Serial.println("MPU6050-Kalibrierung: ");
	if (mpu.WaitForCalibration(40000) != CALIBRATION_SUCCESS)  // R�ckgabewert kann zum Beispiel an Raspberry gesendet werden.
	{
	Serial.println("MPU6050-Kalibrierung gescheitert!");
	}*/

}



