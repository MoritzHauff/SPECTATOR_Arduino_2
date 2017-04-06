// WheelEncoder.h - Moritz Hauff - 06.04.2017

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
	 
	 EncoderInfo lastEncoderInfo;

 public:
	void Init();

	/*Handles the interrupt for this Encoder.*/
	void HandleInterrupt();

	/*Returns the raw number of counts.*/
	int GetCount() { return count; }
	/*Generates new EncoderInfos und resets the values thereafter.*/
	void Update();
	/*Returns the number of counts between the last two calls of 'Update()'.*/
	EncoderInfo GetEncoderInfo() { return lastEncoderInfo; }

	/*Gibt Encoder-Informationen in der seriellen Konsole aus.*/
	void PrintEncoderInfo(EncoderInfo EncoderInfo);
};

#endif
