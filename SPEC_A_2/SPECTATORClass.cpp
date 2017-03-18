/** SPECTATORClass.cpp
***
*** Hier laufen alle andern benötigten Instanzen und die dazugehörigen 
*** Funktionen zusammen.
***
*** Moritz Hauff, 01.03.2017
**/

///////////////////////////////////////////////////////////////////////////
/// Copyright (C) {2017}  {Moritz Hauff}
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// If you start using parts of this code please write a short message to: 
/// license@vierradroboter.de
///
/// If you have any questions contact me via mail: admin@vierradroboter.de
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///Includes
#include "SPECTATORClass.h"

///////////////////////////////////////////////////////////////////////////
///Instanz
SPECTATORClass SA;  // Die HauptInstanz des SPECTATOR-Arduinos.

///////////////////////////////////////////////////////////////////////////
///Konstruktoren 
void SPECTATORClass::Init()
{
	MotorSpeedL = 0;
	MotorSpeedR = 0;
	zielRichtung = 4; //4 bedeutet keine drehung // todo: externe StateMachine zur Steuerung des aktuellen Fahrverhaltens.

	// pinModes
	HeartbeatLED.Init();
	switchLinks.Init();
	switchRechts.Init();

	// Serial communication
	Serial.begin(115200);  // Je höher die Baudrate und je mehr Daten im Serial.print stehen desto mehr Zeit wird gespart.
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

	Motoren.Init();
	Motoren.Kontrolllauf();

	//MPUCalibration();
}

void SPECTATORClass::MPUCalibration()
{
	Serial.println("MPU6050-Kalibrierung: ");
	if (mpu.WaitForCalibration(40000) != CALIBRATION_SUCCESS)  // Rückgabewert kann zum Beispiel an Raspberry gesendet werden.
	{
		Serial.println("MPU6050-Kalibrierung gescheitert!");
	}

	mpu.Update();
	mpuFahrer.SetNorden(mpu.GetYaw());   // hardcode the mpuFahrerCalibration // todo:  should normally done be the RaPi.
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void SPECTATORClass::UpdateSwitches()
{
	//swtLinks = digitalRead2f(switchLinks_Pin);
	//swtRechts = digitalRead2f(switchRechts_Pin);  // auch bei extremst kurzem Betätigen der Switches wird zumindest 2 Ticks lang ihr Status auf "True" gesetzt.

	//serialBuffer.AddMsg(C_SwitchLinks, swtLinks);
	//serialBuffer.AddMsg(C_SwitchRechts, swtRechts);
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

		/*if (zielRichtung != 4)   // this should be done in a ney state.
		{
			if (mpuFahrer.BerechneDrehen(zielRichtung, mpu.GetYaw(), &MotorSpeedL, &MotorSpeedR))  // Drehen beendet
			{
				zielRichtung = 4;
			}
			Motoren.SetMotoren(MotorSpeedL, MotorSpeedR);
		}*/
	}
}
