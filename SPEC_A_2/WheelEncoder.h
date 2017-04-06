// WheelEncoder.h - Moritz Hauff - 06.04.2017

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

#ifndef _WHEELENCODER_h
#define _WHEELENCODER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///EncoderInfo-Struct
struct EncoderInfo
{
	int CountsSinceLastTick;
	long MillisSinceLastTick;
};

///////////////////////////////////////////////////////////////////////////
///WheelEncoderClass
/*todo*/
class WheelEncoderClass
{
 protected:
	 volatile int count;
	 unsigned long lastTime;  // contains the last millis() when the counts where reset.
	 
	 EncoderInfo LastEncoderInfo;

 public:
	void Init();

	/*Handles the interrupt for this Encoder.*/
	void HandleInterrupt();

	/*Returns the raw number of counts.*/
	int GetCount();
	/*Generates new EncoderInfos und resets the values thereafter.*/
	void Update();
	/*Returns the number of counts between the last two calls of 'Update()'.*/
	EncoderInfo GetEncoderInfo();

	/*Gibt Encoder-Informationen in der seriellen Konsole aus.*/
	void PrintEncoderInfo(EncoderInfo EncoderInfo);
};

#endif
