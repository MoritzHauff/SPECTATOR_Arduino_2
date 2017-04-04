// HCSr04.h - Moritz Hauff - 04.04.2017

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

#ifndef _HCSR04_h
#define _HCSR04_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DigitalIO.h"

///////////////////////////////////////////////////////////////////////////
///HCSr04Class
/*Diese Klasse überwacht die HCSr04-Sensoren.*/
class HCSr04Class
{
 protected:
	 uint8_t _echo_Pin;
	 DigitalIOClass *trigPin;  // use the fast IO-functions.

	 long pulsLaenge;
	 int cm;

	 /*Converts the pulsLaenge into cm.*/
	 int convert(long PulsLaenge);

 public:
	 HCSr04Class(uint8_t Echo_Pin, GPIO_pin_t Trig_Pin);
	 ~HCSr04Class();
	 void Init();

	 /*Führt eine Messung aus und speichert diese in der Klasseninstanz.*/
	 void Update();
	 /*Gibt den aktuell gespeicherten Wert zurück.*/
	 int GetDistance();


};

class HCSr04_InterruptClass : HCSr04Class
{
private:
	static void _echo_isr();

	int _trigger, _echo, _max;
	volatile unsigned long _start, _end;
	volatile bool _finished;
	static HCSr04_InterruptClass* _instance;

public:
	HCSr04_InterruptClass(int trigger, int echo, int max_dist = 200);

	void begin();
	void start();
	bool isFinished() { return _finished; }
	unsigned int getDistance();
	static HCSr04_InterruptClass* instance() { return _instance; }

};

#endif
