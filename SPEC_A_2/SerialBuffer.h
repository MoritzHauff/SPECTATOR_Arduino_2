// SerialBuffer.h - Moritz Hauff - 18.02.2017
// see SerialBuffer.cpp

#ifndef _SERIALBUFFER_h
#define _SERIALBUFFER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"

///////////////////////////////////////////////////////////////////////////
///Konstanten
const int MAX_SERIALBUFFER_LENGTH = 120; // max serial buffer msg length. for more information see: http://forum.arduino.cc/index.php?topic=14898.0
#define autoFlush 1

///////////////////////////////////////////////////////////////////////////
///SerialBuffer-Class
/*Die SerialBuffer-Klasse verpackt Sensordaten in ein �bertragungsformat
und schickt sie �ber den seriellen Port an den RaspberryPi.*/
class SerialBuffer
{
protected:
	uint8_t c[MAX_SERIALBUFFER_LENGTH];
	int currentPos;

	void AddMsg(uint8_t Code, String Msg);
public:
	SerialBuffer();

	void AddMsg(uint8_t Code, int Value);
	void AddMsg(uint8_t Code, double Value);

	void Clear();
	void Flush();
};

#endif
