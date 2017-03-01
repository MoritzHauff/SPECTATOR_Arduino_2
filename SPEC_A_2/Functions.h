// Functions.h - Moritz Hauff - 17.02.2017
// see Functions.cpp

#ifndef _FUNCTIONS_h
#define _FUNCTIONS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///Konstanten
#define SERIALBUFF_SIZE 32 // make it big enough to hold your longest command

///////////////////////////////////////////////////////////////////////////
///Functions-Class
/*Beinhaltet s�mtliche sonst nicht zugeordneten Funktionen.*/
class Functions
{
protected:
	void handleReceivedMessage(char *msg);

public:
	void handleSerial();
	
};

extern Functions functions;

#endif
